#include "errors.h"
#include "console.h"

void panic(const char* msg)
{
	print(msg);
	while(1);
}
