#include "console.h"
#include <stdint.h>
#include <stdarg.h>

uint16_t* video_mem = 0;
uint8_t SCR_COL = 0;
uint8_t SCR_ROW = 0; 

uint16_t make_char(char c, char color)
{
	return ((color << 8) | c);
}

void putchar(int x, int y, char c, char color)
{
	video_mem[(y * SCR_WIDTH) + x] = make_char(c, color);	
}

void print_char(char c, char color)
{
	if (c == '\n')
	{
		SCR_ROW += 1;
		SCR_COL = 0;
		return;
	}

	putchar(SCR_COL, SCR_ROW, c, color);
	SCR_COL += 1;

	if (SCR_COL > SCR_WIDTH)
	{
		SCR_COL = 0;
		SCR_ROW += 1;
	}

}

void print(const char* fmt,...)
{
	va_list args;
	va_start(args,  fmt);

	while (*fmt)
	{
		if (*fmt == '%')
		{
		 fmt++;
		
		 switch(*fmt)
		 {
			case 'd': {
				int val = va_arg(args, int);
				print_char(val, DEF_COLOR);
				break;
			}
			case 's': {
				const char* str = va_arg(args, const char*);
				print(str, DEF_COLOR);
				break;
			}
			case 'c':{
				char c = (char)va_arg(args, int);
				print_char(c, DEF_COLOR);
				break;
			}
			default:
				print_char('%', DEF_COLOR);
				print_char(*fmt, DEF_COLOR);
				break;
		 }	
		}
		else
		{
		print_char(*fmt, DEF_COLOR);
		fmt++;
		}
	}
}

void scr_clear()
{
	for (int y = 0; y < SCR_HEIGHT; y++)
	{
		for (int x = 0; x < SCR_WIDTH; x++)
		{
			putchar(x, y, ' ', DEF_COLOR);
		}
	}
}

void scr_init()
{
	video_mem = (uint16_t*)0xB8000;
	scr_clear();
}
