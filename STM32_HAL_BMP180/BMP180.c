//
// Created by AQin on 2023/2/7.
//

#include "BMP180.h"
#include "stdio.h"

#define pow2n(n) (2 << (n - 1))

short AC1, AC2, AC3, B1, B2, MB, MC, MD;
unsigned short AC4, AC5, AC6;

uint8_t OSS = DEAFULT_OSS;

void BMP180_Init()
{
    //读取校准值
    uint8_t AC123_data[6] = {0};
    ReadMem(0xAA, AC123_data, 6);
    AC1 = AC123_data[0] << 8 | AC123_data[1];
    AC2 = AC123_data[2] << 8 | AC123_data[3];
    AC3 = AC123_data[4] << 8 | AC123_data[5];

    uint8_t AC456_data[6] = {0};
    ReadMem(0xB0, AC456_data, 6);
    AC4 = AC456_data[0] << 8 | AC456_data[1];
    AC5 = AC456_data[2] << 8 | AC456_data[3];
    AC6 = AC456_data[4] << 8 | AC456_data[5];

    uint8_t B12_data[4] = {0};
    ReadMem(0xB6, B12_data, 4);
    B1 = B12_data[0] << 8 | B12_data[1];
    B2 = B12_data[2] << 8 | B12_data[3];

    uint8_t M_data[6] = {0};
    ReadMem(0xBA, M_data, 6);
    MB = M_data[0] << 8 | M_data[1];
    MC = M_data[2] << 8 | M_data[3];
    MD = M_data[4] << 8 | M_data[5];

    //printf("\n\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",AC1, AC2, AC3, AC4, AC5, AC6, B1, B2, MB, MC, MD);
}

void BMP180_SetOSS(uint8_t oss)
{
    if (oss >= 4) return; //非法的值
    OSS = oss;
}

long BMP180_ReadUTemp()
{
    WriteByte(0xF4, 0x2E);
    HAL_Delay(5);
    uint8_t raw_data[2] = {0};
    ReadMem(0xF6, raw_data, 2);
    return (raw_data[0] << 8) + raw_data[1];
}

long BMP180_ReadUPres()
{
    WriteByte(0xF4, 0x34 + (OSS << 6));
    HAL_Delay(10);
    uint8_t raw_data[3] = {0};
    ReadMem(0xF6, raw_data, 3);
    return ((raw_data[0] << 16) + (raw_data[1] << 8) + raw_data[2]) >> (8 - OSS);
}

void BMP180_GetData(BMPData* pData)
{
    long UT = BMP180_ReadUTemp();
    long UP = BMP180_ReadUPres();
    pData->UT = UT;
    pData->UP = UP;

    //换算温度
    long X1 = (UT - AC6) * AC5 >> 15;
    long X2 = (MC << 11) / (X1 + MD);
    long B5 = X1 + X2;
    long T = (B5 + 8) / (2 << 3);
    pData->Temp =  T / 10.0;
    OSS = 1;
    //换算气压
    long B6 = B5 - 4000;
    X1 = (B2 * (B6 * B6 / pow2n(12))) / pow2n(11);
    X2 = AC2 * B6 / pow2n(11);
    long X3 = X1 + X2;
    long B3 = (((AC1 * 4 + X3) << OSS) + 2) / 4;
    X1 = AC3 * B6 / pow2n(13);
    X2 = (B1 * (B6 * B6 / pow2n(12))) / pow2n(16);
    X3 = ((X1 + X2) + 2) / 4;
    unsigned long B4 = AC4 * (unsigned long)(X3 + 32768) / pow2n(15);
    unsigned long B7 = ((unsigned long)UP - B3) * (50000 >> OSS);

    long p = 0;
    if (B7 < 0x80000000)
      p = (B7 * 2) / B4;
    else
      p = (B7 / B4) * 2;

    X1 = (p / pow2n(8)) * (p / pow2n(8));
    X1 = (X1 * 3038) / pow2n(16);
    X2 = (-7357 * p) / pow2n(16);
    p = p + (X1 + X2 + 3791) / pow2n(4);

    pData->Pres = p;
}

float BMP180_CalcAbsAltitude(BMPData* pData)
{
    //1 / 5.255 = 0.1902949
    return 44330.0 * (1 - pow(pData->Pres * 1.0 / P0, 0.1902949));
}

float BMP180_CalcPresAtSeaLevel(BMPData* pData, float absAltitude)
{
    return pData->Pres * 1.0 / pow(1 - absAltitude / 44330, 5.255);
}

static void WriteByte(uint8_t Addr, uint8_t Data)
{
    HAL_I2C_Mem_Write(&BMP_IIC, BMP_ADDR, Addr, BMP_IIC_ADDRSIZE, &Data, 1, BMP_TIME_OUT);
}

static uint8_t ReadByte(uint8_t Addr)
{
    uint8_t d = 0;
    HAL_I2C_Mem_Read(&BMP_IIC, BMP_ADDR, Addr, BMP_IIC_ADDRSIZE, &d, 1, BMP_TIME_OUT);
    return d;
}

static void WriteMem(uint8_t Addr, uint8_t* pData, uint16_t Len)
{
    HAL_I2C_Mem_Write(&BMP_IIC, BMP_ADDR, Addr, BMP_IIC_ADDRSIZE, pData, Len, BMP_TIME_OUT);
}

static void ReadMem(uint8_t Addr, uint8_t* pData, uint16_t Len)
{
    HAL_I2C_Mem_Read(&BMP_IIC, BMP_ADDR, Addr, BMP_IIC_ADDRSIZE, pData, Len, BMP_TIME_OUT);
}
