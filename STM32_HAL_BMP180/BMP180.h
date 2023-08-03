//
// Created by AQin on 2023/2/7.
//

#ifndef _BMP180_H
#define _BMP180_H

#include "main.h"
#include "math.h"

#define BMP_ADDR          0xEE                  //设备地址
#define BMP_IIC           hi2c1                 //IIC外设句柄
#define BMP_IIC_ADDRSIZE  I2C_MEMADD_SIZE_8BIT  //使用8位寄存器地址
#define BMP_TIME_OUT      0xffff                //IIC操作超时时间
#define P0                101325                //海平面大气压强

//BMP180采样模式
enum BMP180_OverSampleSetting
{
    BMP180_UltraLowPower = 0,                   //超低功耗
    BMP180_Standard,                            //标准模式
    BMP180_HighResolution,                      //高分辨率
    BMP180_UltraHighResolution                  //超高分辨率
};

#define DEAFULT_OSS       BMP180_Standard

typedef struct
{
    long UT;
    long UP;
    float Temp;
    long Pres;
}BMPData;

void BMP180_Init();
void BMP180_SetOSS(uint8_t oss);
static long BMP180_ReadUTemp();
static long BMP180_ReadUPres();
void BMP180_GetData(BMPData* pData);
float BMP180_CalcAbsAltitude(BMPData* pData);
float BMP180_CalcPresAtSeaLevel(BMPData* pData, float absAltitude);

static void WriteByte(uint8_t Addr, uint8_t Data);
static uint8_t ReadByte(uint8_t Addr);
static void WriteMem(uint8_t Addr, uint8_t* pData, uint16_t Len);
static void ReadMem(uint8_t Addr, uint8_t* pData, uint16_t Len);

#endif //_BMP180_H
