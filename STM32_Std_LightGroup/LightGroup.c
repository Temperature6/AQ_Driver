#include "LightGroup.h"

int current_light = 0;
uint16_t start_pin = GPIO_Pin_0;
uint8_t count = 0;
GPIO_TypeDef* Port;

void Light_Group_Init(GPIO_TypeDef* GPIOx, uint16_t startPin, uint8_t lightCount)
{
	
	//初始化私有变量
	count = lightCount;
	Port = GPIOx;
	start_pin = startPin;
	//初始化GPIO
	GPIO_InitTypeDef pin_init;
	
	
	uint16_t pins = 0;
	
	for (int i = 0; i <= lightCount; i++)
	{
		pins |= startPin << i;
	}
	pin_init.GPIO_Pin = pins;
	pin_init.GPIO_Mode = GPIO_Mode_Out_PP;
	pin_init.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOx, &pin_init);
	
	GPIO_WriteBit(Port, pins, !LIGHT_ON_STATE);
}

void SetLightState(uint8_t lightIndex, uint8_t state)
{
	GPIO_WriteBit(Port, start_pin << lightIndex, state ? Bit_SET : Bit_RESET);
}

void NextLight()
{
	SetLightState(current_light, !LIGHT_ON_STATE);
	current_light++;
	if (current_light >= count)
	{
		current_light = 0;
	}
	SetLightState(current_light, LIGHT_ON_STATE);
}

void PreviousLight()
{
	SetLightState(current_light, !LIGHT_ON_STATE);
	current_light--;
	if (current_light < 0)
	{
		current_light = count - 1;
	}
	SetLightState(current_light, LIGHT_ON_STATE);
}
