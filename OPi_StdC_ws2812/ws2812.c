#include "ws2812.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LOC_1       0xFC
#define LOC_0       0xC0

uint8_t LedsBuffer[LED_LENGTH * 24 + 1];
uint8_t SingleBuffer[24];

void ws2812_Init()
{
    memset(LedsBuffer, LOC_0, LED_LENGTH * 24);
    wiringPiSPISetupMode(SPI_CHANNEL, SPI_PORT, 10000000, 0);
}

void ws2812_SetPixel(uint16_t index, uint8_t R, uint8_t G, uint8_t B)
{
    if (index < 0 || index >= LED_LENGTH)
        return;
    //GRB
    memset(SingleBuffer, LOC_0, 24);
    for (uint8_t i = 0; i < 8; i++)
    {
        SingleBuffer[i + 0 * 8] = (G >> i) & 0x01 ? LOC_1 : LOC_0;
    }
    for (uint8_t i = 0; i < 8; i++)
    {
        SingleBuffer[i + 1 * 8] = (R >> i) & 0x01 ? LOC_1 : LOC_0;
    }
    for (uint8_t i = 0; i < 8; i++)
    {
        SingleBuffer[i + 2 * 8] = (B >> i) & 0x01 ? LOC_1 : LOC_0;
    }
    memcpy(LedsBuffer + index * 24 + 1, SingleBuffer, 24);
}

void ws2812_FillPixel(uint16_t begIndex, uint16_t endIndex, uint8_t R, uint8_t G, uint8_t B)
{
    for (uint16_t i = begIndex; i <= endIndex; i++)
    {
        ws2812_SetPixel(i, R, G, B);
    }
}

void ws2812_Clear()
{
    memset(LedsBuffer, LOC_0, sizeof(LedsBuffer));
}

void ws2812_Show()
{
    LedsBuffer[0] = 0;
    wiringPiSPIDataRW(SPI_CHANNEL, LedsBuffer, sizeof(LedsBuffer));
}

void ws2812_SetPixel2(uint16_t index, const char* RGBformat)
{
    uint8_t R = 0, G = 0, B = 0;
    char R_Str[2] = { RGBformat[0], RGBformat[1] };
    char G_Str[2] = { RGBformat[2], RGBformat[3] };
    char B_Str[2] = { RGBformat[4], RGBformat[5] };

    R = strtoul(R_Str, NULL, 16);
    G = strtoul(G_Str, NULL, 16);
    B = strtoul(B_Str, NULL, 16);

    ws2812_SetPixel(index, R, G, B);
}

void ws2812_FillPixel2(uint16_t begIndex, uint16_t endIndex, const char* RGBformat)
{
    uint8_t R = 0, G = 0, B = 0;
    char R_Str[2] = { RGBformat[0], RGBformat[1] };
    char G_Str[2] = { RGBformat[2], RGBformat[3] };
    char B_Str[2] = { RGBformat[4], RGBformat[5] };

    R = strtoul(R_Str, NULL, 16);
    G = strtoul(G_Str, NULL, 16);
    B = strtoul(B_Str, NULL, 16);

    ws2812_FillPixel(begIndex, endIndex, R, G, B);
}
