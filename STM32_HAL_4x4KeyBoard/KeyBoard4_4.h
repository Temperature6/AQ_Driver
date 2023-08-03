//
// Created by AQin on 2022/11/15.
//

#ifndef INC_4_4KEYBOADR_KEYBOARD4_4_H
#define INC_4_4KEYBOADR_KEYBOARD4_4_H

#include "stm32f1xx_hal.h"

#include "memory.h"

#define IN_GPIO     GPIOA
#define OUT_GPIO    GPIOB
#define IN_PIN      GPIO_PIN_4
#define OUT_PIN     GPIO_PIN_3

extern uint8_t char_map [4][4];

uint8_t GetChar();
void GetString(char* str, int max_len, char end_with);

#endif //INC_4_4KEYBOADR_KEYBOARD4_4_H
