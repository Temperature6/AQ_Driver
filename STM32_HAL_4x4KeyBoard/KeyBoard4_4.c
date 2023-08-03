//
// Created by AQin on 2022/11/15.
//
#include "KeyBoard4_4.h"

uint8_t char_map [4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
};
/**
  * @brief  获取从键盘输入的单个字符
  * @retval 返回输入的字符
  */
uint8_t GetChar() {
  while (1)
  {
    for (int i = 0; i < 4; i++)
    {
      HAL_GPIO_WritePin(IN_GPIO, IN_PIN << i, GPIO_PIN_SET);
      for (int j = 0; j < 4; j++) {
        if (HAL_GPIO_ReadPin(OUT_GPIO, OUT_PIN << j) == GPIO_PIN_SET)
        {
          while (HAL_GPIO_ReadPin(OUT_GPIO, OUT_PIN << j) == GPIO_PIN_SET);
          return char_map[i][j];
        }
      }
      HAL_GPIO_WritePin(IN_GPIO, IN_PIN << i, GPIO_PIN_RESET);
    }
  }
}
/**
  * @brief  获取从键盘输入的字符串
  * @param  str: 用存放字符串的指针
  * @param  max_len: 此字符串发最大长度
  * @param  end_with: 该字符串以哪个字符结束, 输入该字符之后，函数退出
  * @retval None
  */
void GetString(char* str, int max_len, char end_with)
{
  char c;
  int len = 0;
  memset(str, 0, max_len);
  for (int i = 0; i < max_len; i++)
  {
    c = GetChar();
    if (c == end_with)
    {
      *(str + i) = '\0';
      break;
    }
    else if (strlen(str) >= max_len)
    {
      break;
    }
    *(str + i) = c;
  }
}
