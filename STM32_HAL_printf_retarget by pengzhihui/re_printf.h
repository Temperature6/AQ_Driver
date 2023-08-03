//
// Created by 秦璐尧 on 2022/11/29.
//

#ifndef AT24_PRAC_RE_PRINTF_H
#define AT24_PRAC_RE_PRINTF_H

#include "stm32f1xx_hal.h"
#include <sys/stat.h>
#include <stdio.h>

void RetargetInit(UART_HandleTypeDef *huart);

int _isatty(int fd);

int _write(int fd, char *ptr, int len);

int _close(int fd);

int _lseek(int fd, int ptr, int dir);

int _read(int fd, char *ptr, int len);

int _fstat(int fd, struct stat *st);

#endif //AT24_PRAC_RE_PRINTF_H
