#ifndef _DIGHITAL_TUBE_H_
#define _DIGHITAL_TUBE_H_
#include <reg52.h>
#include <stdio.h>
#include <math.h>

#define SEG_PIN 	P0
#define CTRL_PIN	P1

extern unsigned int code LED_Code[];

void ShowDigit(unsigned int digit, unsigned int num);
void ShowInt(unsigned int num);
void ShowFloat(float num);

#endif //_DIGHITAL_TUBE_H_
