#ifndef __LIGHT_GROUP_H_
#define __LIGHT_GROUP_H_
//#define STM32F10X
#define CH32F10X

#ifdef CH32F10X
#include "ch32f10x.h"                   // Device header
#endif //CH32F10X

#ifdef STM32F10X
#include "stm32f10x.h"
#endif //STM32F10X

#define LIGHT_ON_STATE	0	//µãÁÁLEDÊ±µÄIO×´Ì¬

void Light_Group_Init(GPIO_TypeDef* GPIOx, uint16_t startPin, uint8_t lightCount);
void SetLightState(uint8_t lightIndex, uint8_t state);
void NextLight();
void PreviousLight();

#endif //__LIGHT_GROUP_H_
