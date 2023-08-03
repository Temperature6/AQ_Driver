#include "Dighital_tube.h"

typedef unsigned int uint8;

uint8 code LED_Code[] = {
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E
};

int GetWidth(uint8 num)
{
	uint8 digit = 0;
	while (num)
	{
		num /= 10;
		digit++;
	}
	return digit;
}

void ShowDigit(uint8 digit, uint8 num)
{
	CTRL_PIN = ~(0x01 << digit);
	if (num < 10)
		SEG_PIN = ~LED_Code[num];
	else
		SEG_PIN = ~LED_Code[num - 10] | 0x80;
	return;
}

void ShowInt(uint8 num)
{
	uint8 width;
	uint8 i;
	if (!num)
	{
		ShowDigit(0, 0);
	}
	width = GetWidth(num);
	for (i = 0; i < width; i++)
	{
		ShowDigit(i, (int)(num / pow(10, i)) % 10);
	}
	return;
}

void ShowFloat(float num)
{
	
	int i;
	uint8 int_width;
	int_width = GetWidth(num);

	if (int_width == 1)
		num *= 1000;
	else if (int_width == 2)
		num *= 100;
	if (int_width == 3)
		num *= 10;
	for (i = 0; i < 4; i++)
	{
		if (i == 4 - int_width)
			ShowDigit(i, (int)(num / pow(10, i)) % 10+ 10);
		else
			ShowDigit(i, (int)(num / pow(10, i)) % 10);
	}
	return;
}
