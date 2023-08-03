#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "ws2812.h"
//gcc *.c -I. -o ws2812_test -lpthread -lwiringPi && ./ws2812_test
//ln -s /dev/spidev1.0 /dev/spidev0.0
int main()
{
    printf("Hello ws2812\n");
    ws2812_Init();

    int i = 0;
    while (1)
    {
        /* code */
        for (int j = i; j < i + 4; j++)
        {
            ws2812_SetPixel(j % 16, 255, 0, 0);
            ws2812_SetPixel((j + 4) % 16, 0, 255, 0);
            ws2812_SetPixel((j + 8) % 16, 255, 130, 0);
            ws2812_SetPixel((j + 12) % 16, 0, 0, 255);
        }
        i++;
        if (i >= 16)
        {
            i = 0;
        }
        ws2812_Show();
        usleep(100000);
    }
    
}
