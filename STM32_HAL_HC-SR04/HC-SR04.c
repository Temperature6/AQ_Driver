//
// Created by AQin on 2023/1/18.
//

#include "HC-SR04.h"

#define TIM_Stop        HAL_TIM_Base_Stop(&HTIM)
#define TIM_Run         HAL_TIM_Base_Start(&HTIM)
#define TIM_ClearCNT    HTIM.Instance->CNT = 0
#define TIM_GetCNT      HTIM.Instance->CNT

#define Trig_On         HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET)
#define Trig_Off        HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET)
#define Echo_Get        (HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_SET)

float sound_speed = 340;  //声波速度
float temp_c = 15;        //当地温度

void HCSR_Init()
{
  //时钟初始化
  /*
   * 使用STM32CubeMX初始化TIM3
   *
   * 配置TIM3时钟源为内部时钟源
   * 配置TIM3预分频系数为71(Prescaler = 71,系统时钟为72MHz)
   * */
  //IO初始化
  __HAL_RCC_GPIOA_CLK_ENABLE();
  //Trig
  GPIO_InitTypeDef io_init;
  io_init.Pin = TRIG_PIN;
  io_init.Mode = GPIO_MODE_OUTPUT_PP;
  io_init.Pull = GPIO_PULLUP;
  io_init.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(TRIG_PORT, &io_init);
  //Echo
  io_init.Pin = ECHO_PIN;
  io_init.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(ECHO_PORT, &io_init);
}

float HCSR_GetDist_cm()
{
  TIM_Stop;     //停止定时器并清空CNT
  TIM_ClearCNT;

  //发射触发信号
  Trig_Off;
  HCSR_Delay_us(20); //拉低Trig引脚至少15us
  Trig_On;

  //等待Echo引脚被拉高
  while (!Echo_Get);

  TIM_Run;
  while (Echo_Get); //启动定时器，测量Echo引脚回响信号的波长,波长和声波去返的时长成正比
  TIM_Stop;

  return sound_speed * 100 * TIM_GetCNT / 1e6 / 2;
}

static void HCSR_Delay_us(uint16_t us)
{
  TIM_ClearCNT;
  TIM_Run;
  while (us != TIM_GetCNT);
  TIM_Stop;
}

void setSoundSpeed(float sound_speed_mps)
{
  sound_speed = sound_speed_mps;
}

float getSoundSpeed()
{
  return sound_speed;
}

void setTemp_C(float _temp_c)
{
  sound_speed = 332.0 + 0.607 * _temp_c; //空气中声波速度与温度换算
  temp_c = _temp_c;
}

float getTemp_C()
{
  return temp_c;
}
