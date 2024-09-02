#include "string.h"

uint16_t strlen(const char* str)
{
	const char* s = str;
	while (*s) 
		s++;
	
	return s - str;
}

bool strcmp(const char* src, const char* dest)
{
	const char* s1 = src;
	const char* s2 = dest;
	
	// compare character 
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}

	// if both string reached at end, so prev has passed well to and we have same strings
	return *s1 == *s2;
}
