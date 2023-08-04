//
// Created by AQin on 2023/2/8.
//

#include "SSD1306_IIC.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define PAGE_START_ADDR         0xB0
#define LOWER_SATRT_COL_ADDR    0x00
#define UPPER_SATRT_COL_ADDR    0x10

#define BUFFER_SIZE             (128 * 8)

#define MAX(n1, n2)             ((n1) > (n2) ? (n1) : (n2))
#define MIN(n1, n2)             ((n1) < (n2) ? (n1) : (n2))

/*Display Command*/
#define DISPLAY_OFF             0xAE
#define DISPLAY_ON              0xAF

uint8_t OLED_Buffer[BUFFER_SIZE];
char FmtBuff[FMT_BUFF_LEN];

void WriteByte(uint8_t Addr, uint8_t Data);
void WriteMem(uint8_t Addr, uint8_t* pData, uint16_t Len);
void WriteCmd(uint8_t cmd);

void OLED_Init()
{
    /*等待启动*/
    HAL_Delay(100);

    /*初始化*/
    WriteCmd(0xAE);     //关闭显示

    WriteCmd(0xA8);     //设置多路复用比例，默认A8h，63
    WriteCmd(0x3F);

    WriteCmd(0xD3);     //设置显示偏移，默认D3h，0
    WriteCmd(0x00);

    WriteCmd(0x40);     //设置显示起始行
    //水平翻转
#ifdef MIRROR_HORIZONTAL
    WriteCmd(0xA0);
#else
    WriteCmd(0xA1);
#endif //MIRROR_HORIZONTAL
    //垂直翻转
#ifdef MIRROR_VERTICAL
    WriteCmd(0xC0);
#else
    WriteCmd(0xC8);
#endif //MIRROR_VERTICAL
    WriteCmd(0xDA);     //设置COM引脚硬件配置，默认DAh，02
    WriteCmd(0x12);

    WriteCmd(0x81);     //设置对比度控制，默认81h，7Fh
    WriteCmd(0x7F);

    WriteCmd(0xA4);     //设置显示内容来自于GDDRAM

    WriteCmd(0xA6);     //设置不反转颜色

    WriteCmd(0xD5);     //设置晶振频率，默认D5h，80h
    WriteCmd(0x80);

    WriteCmd(0x8D);     //设置电荷泵稳压器，默认8Dh，14h
    WriteCmd(0x14);

    WriteCmd(0xAF);     //打开屏幕


    OLED_ClearScreen();
    OLED_UpdateScreen();
}

 void WriteByte(uint8_t Addr, uint8_t Data)
{
    HAL_I2C_Mem_Write(&OLED_USE_IIC, OLED_ADDR, Addr, OLED_ADDR_SIZE, &Data, 1, OLED_IIC_TIMEOUT);
}

void WriteMem(uint8_t Addr, uint8_t* pData, uint16_t Len)
{
    HAL_I2C_Mem_Write(&OLED_USE_IIC, OLED_ADDR, Addr, OLED_ADDR_SIZE, pData, Len, OLED_IIC_TIMEOUT);
}

void WriteCmd(uint8_t cmd)
{
    WriteByte(0x00, cmd);
}

void OLED_InvertColor(OLED_State state)
{
    if (state == OLED_ENABLE)
        WriteCmd(0xA7);     //反色显示
    else
        WriteCmd(0xA6);     //正常显示
}

void OLED_ClearScreen()
{
    memset(OLED_Buffer, 0x00, BUFFER_SIZE);
}

void OLED_FullScreen()
{
    memset(OLED_Buffer, 0xFF, BUFFER_SIZE);
}

void OLED_UpdateScreen()
{
    for(uint8_t i = 0; i < 64/8; i++)
    {
        WriteCmd(PAGE_START_ADDR + i);
        WriteCmd(LOWER_SATRT_COL_ADDR);
        WriteCmd(UPPER_SATRT_COL_ADDR);
        WriteMem(0x40,&OLED_Buffer[128*i],128);
    }
}

/**
 * @brief
 */
void OLED_Turn(OLED_State state)
{
    if (state == OLED_ENABLE)
        WriteCmd(0xAF);
    else
        WriteCmd(0xAE);
}

void OLED_PutChar(uint8_t x, uint8_t y, char c, MatrixFont font, OLED_Color color)
{
    uint32_t byte;

    //不支持的字符
    if (c < 32 || c > 126)
        return;

    //判断超出屏幕边界
    if (128 < (x + font.width) || 64 < (y + font.height))
        return;

    for(uint32_t i = 0; i < font.height; i++)
    {
        byte = font.bitmap[(c - 32) * font.height + i];     //取一个数据
        for(uint32_t j = 0; j < font.width; j++)
        {
            OLED_DrawPixel(x + j, y + i, ((byte << j) & 0x8000) == 0 ? !color : color);
        }
    }
}

void OLED_DrawPixel(uint8_t x, uint8_t y, OLED_Color color)
{
    if (x >= 128 || y >= 64)    //超出范围
        return;

    uint8_t COM_y = y / 8;
    uint8_t Bit_y = y % 8;

    if (color == White) //白色
        OLED_Buffer[128 * COM_y + x] |= (0x01 << Bit_y);
    else                //黑色
        OLED_Buffer[128 * COM_y + x] &= ~(0x01 << Bit_y);
}

void OLED_DrawBitMap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t width, uint8_t height, OLED_Color color)
{
    uint8_t byte_width = (width + 7) / 8;   //计算图像宽度（bytes）
    uint8_t byte = 0;

    for (int16_t j = 0; j < height; j++, y++)
    {
        for (int16_t i = 0; i < width; i++)
        {
            if (i & 7)
                byte <<= 1;
            else
                byte = bitmap[j * byte_width + i / 8];
            if (byte & 0x80)
                OLED_DrawPixel(x + i, y, color);
        }
    }
}

void OLED_Print(uint8_t x, uint8_t y, char *str, MatrixFont font, OLED_Color color)
{
    uint16_t len = strlen(str);
    uint8_t curX = x, curY = y;

    for (uint16_t i = 0; i < len; i++)
    {
        if (str[i] == '\n')
        {
            curY += font.height;
            curX = x;
            continue;
        }
        OLED_PutChar(curX, curY, str[i], font, color);
        curX += font.width;
    }
}

void OLED_Printf(uint8_t x, uint8_t y, MatrixFont font, OLED_Color color, const char* fmt, ...)
{
    memset(FmtBuff, 0, FMT_BUFF_LEN);
    va_list argList;
    va_start(argList, fmt);
    vsprintf(FmtBuff, fmt, argList);
    va_end(argList);
    OLED_Print(x, y, FmtBuff, font, color);
}

void OLED_SetContrast(uint8_t val)
{
    WriteCmd(0x81);
    WriteCmd(val);
}

void OLED_DrawLine(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, OLED_Color color)
{

    if (start_x == end_x)   //斜率不存在，特殊处理
    {
        uint8_t sy = start_y > end_y ? end_y : start_y;
        uint8_t ey = start_y < end_y ? end_y : start_y;

        for (uint8_t i = sy; i <= ey; i++)
        {
            OLED_DrawPixel(start_x, i, color);
        }
    }
    else
    {
        double k = 1.0 * (start_y - end_y) / (start_x - end_x);
        double b = start_y - k * start_x;

        uint8_t sx = start_x > end_x ? end_x : start_x;
        uint8_t ex = start_x < end_x ? end_x : start_x;

        for (uint8_t i = sx; i <= ex; i++)
        {
            OLED_DrawPixel(i, (uint8_t)(k * i + b), color);
        }
    }
}

void OLED_DrawRectangle1(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, OLED_Color color)
{
    OLED_DrawLine(start_x, start_y, end_x, start_y, color);
    OLED_DrawLine(start_x, start_y, start_x, end_y, color);
    OLED_DrawLine(start_x, end_y, end_x, end_y, color);
    OLED_DrawLine(end_x, start_y, end_x, end_y, color);
}

void OLED_DrawRectangle2(uint8_t start_x, uint8_t start_y, uint8_t width, uint8_t height, OLED_Color color)
{
    OLED_DrawRectangle1(start_x, start_y, start_x + width, start_y + height, color);
}

void OLED_FillRect1(uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y, OLED_Color color)
{
    uint8_t sx = MIN(start_x, end_x);
    uint8_t sy = MIN(start_y, end_y);
    uint8_t ex = MAX(start_x, end_x);
    uint8_t ey = MAX(start_y, end_y);

    for (uint8_t i = sx; i <= ex; i++)
        for (uint8_t j = sy; j <= ey; j++)
            OLED_DrawPixel(i, j, color);
}

void OLED_FillRect2(uint8_t start_x, uint8_t start_y, uint8_t width, uint8_t height, OLED_Color color)
{
    OLED_FillRect1(start_x, start_y, start_x + width, start_y + height, color);
}

void OLED_DrawCircle(uint8_t par_x, uint8_t par_y, uint8_t par_r, OLED_Color par_color)
{
    int32_t x = -par_r;
    int32_t y = 0;
    int32_t err = 2 - 2 * par_r;
    int32_t e2;

    if (par_x >= 128 || par_y >= 64) {
        return;
    }

    do {
        OLED_DrawPixel(par_x - x, par_y + y, par_color);
        OLED_DrawPixel(par_x + x, par_y + y, par_color);
        OLED_DrawPixel(par_x + x, par_y - y, par_color);
        OLED_DrawPixel(par_x - x, par_y - y, par_color);
        e2 = err;

        if (e2 <= y) {
            y++;
            err = err + (y * 2 + 1);
            if(-x == y && e2 <= x) {
                e2 = 0;
            }
        }

        if (e2 > x) {
            x++;
            err = err + (x * 2 + 1);
        }
    } while (x <= 0);
}

/* Draw filled circle. Pixel positions calculated using Bresenham's algorithm */
void OLED_FillCircle(uint8_t par_x, uint8_t par_y, uint8_t par_r, OLED_Color par_color)
{
    int32_t x = - par_r;
    int32_t y = 0;
    int32_t err = 2 - 2 * par_r;
    int32_t e2;

    if (par_x >= 128 || par_y >= 64) {
        return;
    }

    do {
        for (uint8_t _y = (par_y + y); _y >= (par_y - y); _y--) {
            for (uint8_t _x = (par_x - x); _x >= (par_x + x); _x--) {
                OLED_DrawPixel(_x, _y, par_color);
            }
        }

        e2 = err;
        if (e2 <= y) {
            y++;
            err = err + (y * 2 + 1);
            if (-x == y && e2 <= x) {
                e2 = 0;
            }
        }

        if (e2 > x) {
            x++;
            err = err + (x * 2 + 1);
        }
    } while (x <= 0);
}
