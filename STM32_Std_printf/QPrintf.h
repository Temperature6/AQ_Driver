#ifndef _QPRINTF_H_
#define _QPRINTF_H_

#include "stm32f10x.h"                  // Device header
#include <stdio.h>

#define REC_INT_ENABLE 0

void Serial_Init(uint16_t baudRate);
void Serial_SendByte(uint8_t byte);


#endif //_QPRINTF_H_
