//
// Created by AQin on 2022/12/7.
//

#ifndef MPU6050_PRAC_MPU6050_H
#define MPU6050_PRAC_MPU6050_H

#include "main.h"
#include "memory.h"
#include "math.h"

#define DEV_ADD           0xd0            //设备地址
#define IIC               hi2c1           //IIC外设句柄
#define TIME_OUT          100             //IIC读写超时
#define LOCAL_GRAVITY     9.8f            //当地重力加速度值
#define CORRECT_OFFSET    1               //是否在初始化时纠正偏移
#define CORR_SAMP_CIRCLE  10              //纠偏时采样次数
#define RAD_TO_DEG        (180 / 3.1415f) //弧度转角度
#define DEG_TO_RAD        (3.1415f / 180) //角度转弧度

typedef struct MPUDATA
{
    double Accl_X, Accl_Y, Accl_Z;
    double Gyro_X, Gyro_Y, Gyro_Z;
    double Pitch; //俯仰角
    double Roll;  //横滚角
    //double Yaw;   //偏航角
    double Temp;
    uint8_t success;
}Mpudata;

/*Private Function*/
static uint8_t WriteMem(uint8_t add, uint8_t* data, uint8_t len);
static uint8_t ReadMem(uint8_t add, uint8_t* data, uint8_t len);
static uint8_t WriteByte(uint8_t add, uint8_t data);
static uint8_t ReadByte(uint8_t add, uint8_t* data);
/*Public Function*/
uint8_t MPU6050_Init();
void MPU6050_ReadData(Mpudata* data);

#endif //MPU6050_PRAC_MPU6050_H
