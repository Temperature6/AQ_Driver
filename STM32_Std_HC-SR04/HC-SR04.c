#include "HC-SR04.h"
#include "stdio.h"

#ifdef STM32F103
#include "stm32f10x.h"                  // Device header
#endif //STM32F103
#ifdef CH32F103
#include "ch32f10x.h"                  	// Device header
#endif //CH32F103

#define TIMx		TIM3
#define GPIOx		GPIOA
#define Trig		GPIO_Pin_0
#define Echo		GPIO_Pin_1

#define ClearCNT	TIM_SetCounter(TIMx, 0)
#define GetCNT		TIM_GetCounter(TIMx)
#define TIM_Run		TIM_Cmd(TIMx, ENABLE)
#define TIM_Stop	TIM_Cmd(TIMx, DISABLE)
#define Trig_High	GPIO_WriteBit(GPIOx, Trig, Bit_SET)
#define Trig_Low	GPIO_WriteBit(GPIOx, Trig, Bit_RESET)
#define ReadEcho	GPIO_ReadInputDataBit(GPIOx, Echo)

float soundSpeed_cms = 34000;
float tempc = 15;

void HCSR_Init()
{
	//定时器初始化
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_InternalClockConfig(TIMx);
	
	TIM_TimeBaseInitTypeDef tim_init;
	tim_init.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_init.TIM_CounterMode = TIM_CounterMode_Up;
	tim_init.TIM_Period = 65535;
	tim_init.TIM_Prescaler = 71; //记一个数的时间：0.000001s 0.001ms 1us
	tim_init.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIMx, &tim_init);
	TIM_ClearFlag(TIMx,TIM_FLAG_Update);
	ClearCNT;
	
	//IO初始化
	GPIO_InitTypeDef io_init;
	io_init.GPIO_Speed = GPIO_Speed_50MHz;
	
	//Trig
	io_init.GPIO_Pin = Trig;
	io_init.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOx, &io_init);
	Trig_Low;
	
	//Echo
	io_init.GPIO_Pin = Echo;
	io_init.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOx, &io_init);
}

float HCSR_Read()
{
	Trig_Low;
	delay_us(15);
	Trig_High;
	
	while (!ReadEcho);
	TIM_Run;
	while (ReadEcho);
	TIM_Stop;
	
	return GetCNT * soundSpeed_cms / 1e6 / 2;
}

static void delay_us(uint16_t xus)
{
	ClearCNT;
	TIM_Run;
	while (GetCNT != xus);
	TIM_Stop;
}

void HCSR_setSoundSpeed(float inSpeed)
{
	soundSpeed_cms = inSpeed;
}

float HCSR_getSoundSpeed()
{
	return soundSpeed_cms;
}

void HCSR_setTemp(float tc)
{
	tempc = tc;
}

float HCSR_getTemp()
{
	return tempc;
}
