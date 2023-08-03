//
// Created by AQin on 2023/1/31.
//

#ifndef _WS2812B_H
#define _WS2812B_H
#include "main.h"
#include "system_stm32f1xx.h"

/*
����ҵ�ʱ�Ӳ���72MHz��ô�죿

1)	ws2812ͨ���߼���ο�
	[DataSheet]https://www.semiee.com/file/Worldsemi/Worldsemi-WS2812B.pdf
2)	__NOP()ռ��һ��ʱ�����ڣ����㷽��Ϊ 1 * 1 / ClockSpeed (s)

*/
#warning "Clock Speed MUST Be 72MHz"	//�˽�֮�����ע�͵�

#define WS2812_LENGTH   64				//���WS2812����

/*
	@brief: ��ʼ��WS2812�ƴ�
	@param:
		Port: GPIOA/GPIOB/GPIOC...
		Pin: GPIO_PIN_x
*/
void ws2812_Init(GPIO_TypeDef* Port, uint16_t Pin);

/*
	@brief: ����һ���Ƶ���ɫ���������֮��Ʋ�������������Ҫ����"ws2812_show()"ˢ����ʾ
	@param:
		index: Ҫ���õĵƵ���������0��ʼ��
		R: RGBɫ�ʺ�ɫ����
		G: RGBɫ����ɫ����
		B: RGBɫ����ɫ����
*/
void ws2812_setPixel(uint16_t index, uint8_t R, uint8_t G, uint8_t B);

/*
	@brief: ��ָ����ɫ�����һ���ֵƴ�
	@param:
		beg_index: ָ����ʼ���ĵƵ�����
		count: ָ��������
		R: RGBɫ�ʺ�ɫ����
		G: RGBɫ����ɫ����
		B: RGBɫ����ɫ����
*/
void ws2812_fillPixel(uint16_t beg_index, uint16_t count, uint8_t R, uint8_t G, uint8_t B);

/*
	@brief: ���ȫ���Ƶ�״̬
*/

void ws2812_clearAll();

/*
	@brief: ˢ����ʾ
*/
void ws2812_show();


#endif //_WS2812B_H
