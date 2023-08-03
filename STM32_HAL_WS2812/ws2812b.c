//
// Created by AQin on 2023/1/31.
//

#include "ws2812b.h"

#define IO_Off  GPIOx->BSRR = (uint32_t)Pinx << 16u;        //IO置低电平
#define IO_On   GPIOx->BSRR = Pinx;                         //IO置高电平
//延时一段时间（对于72MHz的MCU，延时138ns），可根据时钟速度自行设置，
#define NOP_138ns {__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();}

uint32_t data_array[WS2812_LENGTH]; //灯带数据
uint16_t max_ = 0;                  //需要控制的最大范围
GPIO_TypeDef *GPIOx;                //GPIOx
uint16_t Pinx;                      //GPIO_PIN_x

void ws2812_Init(GPIO_TypeDef* Port, uint16_t Pin)
{
  //开启时钟
  if (Port == GPIOA)      __HAL_RCC_GPIOA_CLK_ENABLE();
  else if (Port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
  else if (Port == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
  else if (Port == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
  else if (Port == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();
  else return;

  GPIOx = Port;
  Pinx = Pin;

  GPIO_InitTypeDef IO_Init;
  IO_Init.Pin = Pin;
  IO_Init.Mode = GPIO_MODE_OUTPUT_PP;
  IO_Init.Pull = GPIO_PULLDOWN;
  IO_Init.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(Port, &IO_Init);

  IO_On;
  IO_Off;
  HAL_Delay(1);
  IO_On;
}

void ws2812_setPixel(uint16_t index, uint8_t R, uint8_t G, uint8_t B)
{
  //数据拼接
  uint32_t datau24 = G;
  datau24 <<= 8;
  datau24 |= R;
  datau24 <<= 8;
  datau24 |= B;

  data_array[index] = datau24;

  max_ = index;
}

void ws2812_show()
{
  //发送RESET
  IO_On;
  IO_Off;
  HAL_Delay(1);
  IO_On;
  IO_Off;

  for (uint16_t i = 0; i <= max_; i++)
  {
    uint32_t cur = data_array[i];
    for (uint8_t b = 0; b < 24; b++)
    {
      if ((cur << b) & 0x800000)
      {
        /*Send 1*/
        IO_On;
        NOP_138ns; NOP_138ns;NOP_138ns; NOP_138ns; NOP_138ns;   //T1H 580~1000ns
        IO_Off;
        NOP_138ns; NOP_138ns;NOP_138ns; NOP_138ns; NOP_138ns;  //T1L 580~1000ns
      }
      else
      {
        /*Send 1*/
        IO_On;
        NOP_138ns; NOP_138ns;                                  //T0H 220~380ns
        IO_Off;
        NOP_138ns; NOP_138ns;NOP_138ns; NOP_138ns; NOP_138ns;  //T1L 580~1000ns
      }
      IO_Off;
    }
  }
  IO_On;
  IO_Off;
  HAL_Delay(1);
  IO_On;
}

void ws2812_fillPixel(uint16_t beg_index, uint16_t count, uint8_t R, uint8_t G, uint8_t B)
{
  for (uint16_t i = beg_index; i < beg_index + count; i++)
    ws2812_setPixel(i, R, G, B);
}

void ws2812_clearAll()
{
  ws2812_fillPixel(0, WS2812_LENGTH, 0, 0, 0);
}
