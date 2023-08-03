#ifndef _HC_SR04_H_
#define _HC_SR04_H_

#define STM32F103

#ifdef STM32F103
#include "stm32f10x.h"                  // Device header
#endif //STM32F103
#ifdef CH32F103
#include "ch32f10x.h"                  	// Device header
#endif //CH32F103

void HCSR_Init();
void HCSR_setSoundSpeed(float inSpeed);
float HCSR_getSoundSpeed();
void HCSR_setTemp(float tc);
float HCSR_getTemp();
float HCSR_Read();

static void delay_us(uint16_t xus);


#endif //_HC_SR04_H_
