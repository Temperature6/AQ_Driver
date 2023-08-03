//
// Created by AQin on 2022/12/7.
//
#include "MPU6050.h"

uint8_t Accel_raw[6];
uint8_t Gyro_raw[6];
uint8_t Temp_raw[2];

uint8_t init = 1;

double K_Gyro, K_Accel;

double offset[8];

enum
{
    DEVICES_RESET           = 0x80, //电源管理寄存器1:设备重置
    DEVICES_SLEEP           = 0x40, //电源管理寄存器1:设备休眠
    DEVICES_DIS_TEMP        = 0x08, //电源管理寄存器1:禁用温度传感器
    X_AXIS_SELF_CHECK       = 0x80, //加速度计配置寄存器&陀螺仪配置寄存器:X轴自检
    Y_AXIS_SELF_CHECK       = 0x40, //加速度计配置寄存器&陀螺仪配置寄存器:Y轴自检
    Z_AXIS_SELF_CHECK       = 0x20, //加速度计配置寄存器&陀螺仪配置寄存器:Z轴自检
    ACC_FULL_RANGE_2G       = 0x00, //加速度计配置寄存器:满量程±2G
    ACC_FULL_RANGE_4G       = 0x08, //加速度计配置寄存器:满量程±4G
    ACC_FULL_RANGE_8G       = 0x10, //加速度计配置寄存器:满量程±8G
    ACC_FULL_RANGE_16G      = 0x18, //加速度计配置寄存器:满量程±16G
    GYRO_FULL_RANGE_250     = 0x00, //陀螺仪配置寄存器:满量程250°/s
    GYRO_FULL_RANGE_500     = 0x08, //陀螺仪配置寄存器:满量程500°/s
    GYRO_FULL_RANGE_1000    = 0x10, //陀螺仪配置寄存器:满量程1000°/s
    GYRO_FULL_RANGE_2000    = 0x18, //陀螺仪配置寄存器:满量程2000°/s
    ACCEL_XOUT_H            = 0x3b, //X轴加速度高8位输出寄存器(6 byte)
    TEMP_OUT_H              = 0x41, //温度高8位寄存器(2 byte)
    GYRO_XOUT_H             = 0x43 //X轴角速度高8位寄存器(6 byte)
};

uint8_t MPU6050_Init()
{
  uint8_t init_res = 0;
  /* 地址:0x6B
   * 名称:电源管理寄存器1
   * 操作:R/W
   * 描述:
   * bit7:设备重置
   * bit6:低功耗休眠模式
   * bit5:bit6置0，bit5置1，设备在休眠和工作模式之间切换，切换速度由电源管理寄存器2配置
   * bit4:-
   * bit3:禁用温度传感器
   * bit2~0:时钟源选择
   * */
  if (WriteByte(0x6b, 0) == HAL_OK) init_res |= 0x01;

  /* 地址:0x19
   * 名称:采样分频(SMPLRT_DIV)寄存器
   * 操作:R/W
   * 描述:采样频率=陀螺仪输出频率/(SMPLRT_DIV + 1)
   * 当 DLPF(数字低通滤波器，见寄存器Configuration)禁用时(DLPF_CFG=0 or 7),陀螺输出频率=8kHz；
   * 当 DLPF 使能，陀螺仪输出频率=1KHz
   * 注意：加速度计输出频率为 1KHz。这意味着，当采样频率大于1KHZ时，同个加速度计采样得到的数据，可能不止一次输出到FIFO、DMP、传感器寄存器。
   * */
  if (WriteByte(0x19, 0x07) == HAL_OK) init_res |= 0x02;

  /* 地址:0x1c
   * 名称:加速度计配置寄存器 Accelerometer Configuration
   * 操作:R/W
   * 描述:
   * bit7:X轴自检
   * bit6:Y轴自检
   * bit5:Z轴自检
   * bit4~3:加速度计满量程范围 0->±2g; 1->±4g; 2->±8g; 3->±16g;
   * bit2~0:ACCEL_HPF
   * */
  if (WriteByte(0x1c, X_AXIS_SELF_CHECK | Y_AXIS_SELF_CHECK | Z_AXIS_SELF_CHECK) == HAL_OK) init_res |= 0x04;
  //计算加速度计转化系数K_Accel
  //K_Accel = 65536 / [量程长度] (LSB / g)
  K_Accel = 65536.0 / 4;

  /* 地址:0x1b
   * 名称:陀螺仪配置寄存器 Gyroscope Configuration
   * 操作:R/WA
   * 描述:
   * bit7:X轴自检
   * bit6:Y轴自检
   * bit5:Z轴自检
   * bit4~3:陀螺仪满量程范围 0->±250°/s; 1->±500°/s; 2->±1000°/s; 3->±2000°/s
   * bit2~0:-
   * */
  if (WriteByte(0x1b, GYRO_FULL_RANGE_2000) == HAL_OK) init_res |= 0x08;
  //计算陀螺仪转化系数K_Gyro
  //G = 65536 / [量程长度] (LSB / (°/s))
  K_Gyro = 65536.0 / 4000;

#if CORRECT_OFFSET == 1 //纠偏
  Mpudata data;
  for (uint8_t i = 0; i < CORR_SAMP_CIRCLE; i++)
  {
    MPU6050_ReadData(&data);
    offset[0] += data.Accl_X;
    offset[1] += data.Accl_Y;
    offset[2] += data.Accl_Z;
    offset[3] += data.Gyro_X;
    offset[4] += data.Gyro_Y;
    offset[5] += data.Gyro_Z;

    double accX = (int16_t)((Accel_raw[0] << 8) | Accel_raw[1]);
    double accY = (int16_t)((Accel_raw[2] << 8) | Accel_raw[3]);
    double accZ = (int16_t)((Accel_raw[4] << 8) | Accel_raw[5]);
    offset[6] += atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG * 2;
    offset[7] += atan2(-accX, accZ) * RAD_TO_DEG * 2;
  }
  for (uint8_t j = 0; j < 8; j++)
  {
    offset[j] /= (double)CORR_SAMP_CIRCLE;
  }
  init = 0;
#endif

  return init_res;
}

void MPU6050_ReadData(Mpudata* data){
  //读取生数据
  if (HAL_OK != ReadMem(ACCEL_XOUT_H, Accel_raw, 6))
  {
    data->success = 0;
  }
  if (HAL_OK != ReadMem(TEMP_OUT_H, Temp_raw, 2))
  {
    data->success = 0;
  }
  if (HAL_OK != ReadMem(GYRO_XOUT_H, Gyro_raw, 6))
  {
    data->success = 0;
  }
  //处理数据
  double accX = (int16_t)((Accel_raw[0] << 8) | Accel_raw[1]);
  double accY = (int16_t)((Accel_raw[2] << 8) | Accel_raw[3]);
  double accZ = (int16_t)((Accel_raw[4] << 8) | Accel_raw[5]);

  data->Accl_X = LOCAL_GRAVITY * accX / K_Accel;
  data->Accl_Y = LOCAL_GRAVITY * accY / K_Accel;
  data->Accl_Z = LOCAL_GRAVITY * accZ / K_Accel;

  data->Roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG * 2;
  data->Pitch = atan2(-accX, accZ) * RAD_TO_DEG * 2;



  data->Gyro_X = (int16_t)((Gyro_raw[0] << 8) | Gyro_raw[1]) / K_Gyro;
  data->Gyro_Y = (int16_t)((Gyro_raw[2] << 8) | Gyro_raw[3]) / K_Gyro;
  data->Gyro_Z = (int16_t)((Gyro_raw[4] << 8) | Gyro_raw[5]) / K_Gyro;

  data->Temp = (int16_t)((Temp_raw[0] << 8) | Temp_raw[1]) / 340.0 + 36.5;

#if CORRECT_OFFSET == 1 //纠偏
  data->Accl_X -= init ? 0 : offset[0];
  data->Accl_Y -= init ? 0 : offset[1];
  data->Accl_Z -= init ? 0 : offset[2];
  data->Gyro_X -= init ? 0 : offset[3];
  data->Gyro_Y -= init ? 0 : offset[4];
  data->Gyro_Z -= init ? 0 : offset[5];
  data->Roll -= init ? 0 : offset[6];
  data->Pitch -= init ? 0 : offset[7];
#endif //CORRECT_OFFSET == 1
}

static uint8_t WriteMem(uint8_t add, uint8_t* data, uint8_t len)
{
  return HAL_I2C_Mem_Write(&IIC,
                           DEV_ADD,
                           add,
                           I2C_MEMADD_SIZE_8BIT,
                           data,
                           len,
                           TIME_OUT);
}

static uint8_t ReadMem(uint8_t add, uint8_t* data, uint8_t len)
{
  return HAL_I2C_Mem_Read(&IIC,
                          DEV_ADD,
                          add,
                          I2C_MEMADD_SIZE_8BIT,
                          data,
                          len,
                          TIME_OUT);
}

static uint8_t WriteByte(uint8_t add, uint8_t data)
{
  return WriteMem(add, &data, 1);
}

static uint8_t ReadByte(uint8_t add, uint8_t* data)
{
  return ReadMem(add, data, 1);
}
