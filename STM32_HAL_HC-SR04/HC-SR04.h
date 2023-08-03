//
// Created by AQin on 2023/1/18.
//

#ifndef HCSR04_HC_SR04_H
#define HCSR04_HC_SR04_H
#include "main.h"

#define HTIM        htim3         //用于计时的时钟句柄
#define TRIG_PORT   GPIOA         //Trig线连接的GPIOx(默认如此，更改之后自行开启对应的GPIO的时钟)
#define ECHO_PORT   GPIOA         //Echo线连接的GPIOx(默认如此，更改之后自行开启对应的GPIO的时钟)
#define TRIG_PIN    GPIO_PIN_0    //Trig线连接的GPIO_PIN_x
#define ECHO_PIN    GPIO_PIN_1    //Echo线连接的GPIO_PIN_x

void HCSR_Init();
float HCSR_GetDist_cm();
void setSoundSpeed(float sound_speed_mps);
float getSoundSpeed();
void setTemp_C(float _temp_c);
float getTemp_C();
static void HCSR_Delay_us(uint16_t us);

#endif //HCSR04_HC_SR04_H
