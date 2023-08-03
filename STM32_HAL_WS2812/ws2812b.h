//
// Created by AQin on 2023/1/31.
//

#ifndef _WS2812B_H
#define _WS2812B_H
#include "main.h"
#include "system_stm32f1xx.h"

/*
如果我的时钟不是72MHz怎么办？

1)	ws2812通信逻辑请参考
	[DataSheet]https://www.semiee.com/file/Worldsemi/Worldsemi-WS2812B.pdf
2)	__NOP()占用一个时钟周期，计算方法为 1 * 1 / ClockSpeed (s)

*/
#warning "Clock Speed MUST Be 72MHz"	//了解之后可以注释掉

#define WS2812_LENGTH   64				//最大WS2812个数

/*
	@brief: 初始化WS2812灯带
	@param:
		Port: GPIOA/GPIOB/GPIOC...
		Pin: GPIO_PIN_x
*/
void ws2812_Init(GPIO_TypeDef* Port, uint16_t Pin);

/*
	@brief: 设置一个灯的颜色，设置完成之后灯不会立刻亮起，需要调用"ws2812_show()"刷新显示
	@param:
		index: 要设置的灯的索引（从0开始）
		R: RGB色彩红色分量
		G: RGB色彩绿色分量
		B: RGB色彩蓝色分量
*/
void ws2812_setPixel(uint16_t index, uint8_t R, uint8_t G, uint8_t B);

/*
	@brief: 用指定的色彩填充一部分灯带
	@param:
		beg_index: 指定开始填充的灯的索引
		count: 指定填充个数
		R: RGB色彩红色分量
		G: RGB色彩绿色分量
		B: RGB色彩蓝色分量
*/
void ws2812_fillPixel(uint16_t beg_index, uint16_t count, uint8_t R, uint8_t G, uint8_t B);

/*
	@brief: 清空全部灯的状态
*/

void ws2812_clearAll();

/*
	@brief: 刷新显示
*/
void ws2812_show();


#endif //_WS2812B_H
