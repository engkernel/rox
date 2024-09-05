#include <stdint.h>
#include "console.h"
#include "kheap.h"

void kernel_start()
{
	scr_init();
	kheap_init();
	void* ptr = kmalloc(1252);
	if (ptr)
	{
	}
}
