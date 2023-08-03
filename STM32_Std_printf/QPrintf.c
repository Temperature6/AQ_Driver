#include "QPrintf.h"
#include <stdio.h>

void Serial_Init(uint16_t baudRate)
{
	//开启USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//配置需要的GPIO(对于USART1，PA9-TX，PA10-RX)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//配置GPIO，TX
	GPIO_InitTypeDef gpio_init;
	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_init.GPIO_Pin = GPIO_Pin_9;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_init);
	
	//配置GPIO，RX
	gpio_init.GPIO_Mode = GPIO_Mode_IPU;
	gpio_init.GPIO_Pin = GPIO_Pin_10;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio_init);
	
	//初始化USART
	USART_InitTypeDef usart_init;
	usart_init.USART_BaudRate = baudRate;
	usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart_init.USART_Parity = USART_Parity_No;
	usart_init.USART_StopBits = USART_StopBits_1;
	usart_init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &usart_init);
	
#if (REC_INT_ENABLE == 1)
	//开启中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef nvic_init;
	nvic_init.NVIC_IRQChannel = USART1_IRQn;
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_init.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvic_init);
#endif //REC_INT_ENABLE == 1
	
	
	USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t byte)
{
	USART_SendData(USART1, byte);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

uint8_t Serial_ReadByte()
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART1);
}

int fputc(int ch, FILE* f)
{
	Serial_SendByte(ch);
	return ch;
}

int fgetc(FILE *f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(USART1);
}

#if (REC_INT_ENABLE == 1)
void USART1_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART1, USART_IT_RXNE) == SET)
	{
		/*User Code Begin*/
		printf("%c", Serial_ReadByte() + 1);
		/*User Code End*/
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
#endif //REC_INT_ENABLE == 1
