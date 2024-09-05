#include <stdint.h>
#include "helper/console.h"
#include "mm/kheap.h"

void kernel_start()
{
	scr_init();
	kheap_init();
	void* ptr = kmalloc(1252);
	if (ptr != NULL)
	{
		print("ptr allocated");
		kfree(ptr);
	}
}
