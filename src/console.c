#include "console.h"
#include <stdint.h>

uint16_t* video_mem = 0;
uint8_t SCR_COL = 0;
uint8_t SCR_ROW = 0; 

uint16_t make_char(char c, char colour)
{
	return ((colour << 8) | c);
}

void putchar(int x, int y, char c, char colour)
{
	video_mem[(y * SCR_WIDTH) + x] = make_char(c, colour);	
}

void write_char(char c, char colour)
{
	putchar(SCR_COL, SCR_ROW, c, colour);
	SCR_COL += 1;

	if (c == '\n')
	{
		SCR_ROW += 1;
	}

	if (SCR_COL >= SCR_WIDTH)
	{
		SCR_COL = 0;
		SCR_ROW += 1;
	}
}

void print(const char* str)
{
	while (*str != '\0')
	{
		write_char(*str, 10);
		str++;
	}
}

void scr_clear()
{
	for (int x = 0; x <= SCR_WIDTH; x++)
	{
		for (int y = 0; y <= SCR_HEIGHT; y++)
		{
			putchar(x, y, ' ', 0);
		}
	}
}

void scr_init()
{
	video_mem = (uint16_t*)0xB8000;
	scr_clear();
	print("Hello World!");
}
