/*
 Software IIC Driver Coded By AQ
*/
#ifndef _SOFT_IIC_H_
#define _SOFT_IIC_H_
#include "stm32f10x.h"                  // Device header

#define DELAY_ENABLE					//�Ƿ��ڸߵ͵�ƽ�仯֮���ӳ�

#ifdef DELAY_ENABLE
#define DELAY_VALUE_US	10				//�ߵ͵�ƽ�仯֮���ӳٵ�ʱ�䣨΢�룩����delay������STM32F1@72MHz
#endif //DELAY_ENABLE

#define SOFT_PORT	GPIOB				//SDA��SCL������GPIOx
#define SDA_PIN		GPIO_Pin_7			//SDA��
#define SCL_PIN		GPIO_Pin_6			//SCL��

enum SoftIIC_State
{
	SoftIIC_OK = 0,
	SoftIIC_Fail,
};

enum SoftIIC_AddrSize
{
	SoftIIC_AddrSize_8Bit = 8,
	SoftIIC_AddrSize_16Bit = 16
};

/* IIC���� */
void SoftIIC_Init();

uint8_t isDeviceReady(uint8_t addr);

uint8_t SoftIIC_WriteByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t RegAddrSize, uint8_t Data);
uint8_t SoftIIC_ReadByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t RegAddrSize, uint8_t* pData);

uint8_t SoftIIC_WriteMem(uint8_t DevAddr, uint8_t RegAddr, uint8_t RegAddrSize, uint8_t* pData, uint16_t Len);
uint8_t SoftIIC_ReadMem(uint8_t DevAddr, uint8_t RegAddr, uint8_t RegAddrSize, uint8_t* pData, uint16_t Len);


/* �ײ�ͨ�Ų��� */
static void Start();
static void Stop();
static void Send_Byte(uint8_t byte);
static uint8_t Recv_Byte();
static void Send_ACK(uint8_t Bit_ACK);
static uint8_t Recv_ACK();
/* ���Ų��� */
static void SCL_Write(uint8_t state);
static void SDA_Write(uint8_t state);
static uint8_t SDA_Read();
/* ���� */
static void delay_us(uint32_t xus);
void SoftIIC_Test();

#endif //_SOFT_IIC_H_
