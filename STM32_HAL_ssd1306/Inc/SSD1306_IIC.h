//
// Created by AQin on 2023/2/8.
//

#ifndef OLED_IIC_H
#define OLED_IIC_H

#include "main.h"
#include "MatrixFont.h"

#define OLED_USE_IIC                hi2c1
#define OLED_ADDR                   0x78
#define OLED_ADDR_SIZE              I2C_MEMADD_SIZE_8BIT
#define OLED_IIC_TIMEOUT            0xffff
#define FMT_BUFF_LEN                64
#define S                           SSD1306
//#define MIRROR_VERTICAL
//#define MIRROR_HORIZONTAL

extern I2C_HandleTypeDef            OLED_USE_IIC;

typedef enum
{
    White = 1,
    Black = !White
}OLED_Color;

typedef enum
{
    OLED_ENABLE = 0,
    OLED_DISABLE = !OLED_ENABLE
}OLED_State;

/*Public Function*/
void OLED_Init();

/* Graphic Function */
//Fill类
void OLED_FillCircle(uint8_t center_x, uint8_t center_y, uint8_t r, OLED_Color color);
void OLED_FillRect1(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, OLED_Color color);
void OLED_FillRect2(uint8_t start_x, uint8_t start_y, uint8_t width, uint8_t height, OLED_Color color);
//Draw类
void OLED_DrawRectangle1(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, OLED_Color color);
void OLED_DrawRectangle2(uint8_t start_x, uint8_t start_y, uint8_t width, uint8_t height, OLED_Color color);
void OLED_DrawLine(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, OLED_Color color);
void OLED_DrawCircle(uint8_t center_x, uint8_t center_y, uint8_t r, OLED_Color color);
void OLED_DrawBitMap(uint8_t x, uint8_t y, const uint8_t* bitmap, uint8_t width, uint8_t height, OLED_Color color);
void OLED_DrawPixel(uint8_t x, uint8_t y, OLED_Color color);
//Print类
void OLED_Print(uint8_t x, uint8_t y, char* str, MatrixFont font, OLED_Color color);
void OLED_Printf(uint8_t x, uint8_t y, MatrixFont font, OLED_Color color, const char* fmt, ...);
void OLED_PutChar(uint8_t x, uint8_t y, char c, MatrixFont font, OLED_Color color);
//屏幕类
void OLED_ClearScreen();
void OLED_FullScreen();
void OLED_UpdateScreen();

//Display Function
void OLED_SetContrast(uint8_t val);
void OLED_Turn(OLED_State state);
void OLED_InvertColor(OLED_State state);

#endif //OLED_IIC_H
