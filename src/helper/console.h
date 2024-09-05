#ifndef CONSOLE_H
#define CONSOLE_H

#define SCR_WIDTH 80
#define SCR_HEIGHT 80

#define DEF_COLOR 15

void scr_clear();
void print(const char* str, ...);
void scr_init();

#define pr_debug(fmt, ...) print("DEBUG: " fmt " " ,##__VA_ARGS__)
#endif
