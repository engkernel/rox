#include "kheap.h"
#include "mm/memory.h"
#include "mm/frame.h"
#include "helper/errors.h"
#include "helper/console.h"

struct frames kernel_frames;
struct frame_table kernel_frame_table;

void kheap_init()
{
	kernel_frame_table.entries = (FRAME_TABLE_ENTRY*)(FRAME_TABLE_ADDRESS);
	kernel_frame_table.total = FRAME_PHYSICAL_SIZE_BYTE / FRAME_SIZE;

	void* last_frame = (void*)FIRST_FRAME_ADDRESS + FRAME_PHYSICAL_SIZE_BYTE;
	int res = kernel_frames_create(&kernel_frames, (void*)(FIRST_FRAME_ADDRESS), last_frame, &kernel_frame_table);
	if (res < 0)
	{
		panic("frames manager failed");
	}
}

void* kmalloc(size_t size)
{
	return kernel_frame_alloc(&kernel_frames, size);
}

void* kzalloc(size_t size)
{
	void* ptr = kmalloc(size);
	if (!ptr)
		return 0;

	memset(ptr, 0x00, size);
	return ptr;
}

void kfree(void* ptr)
{
	kernel_frame_free(&kernel_frames, ptr);
}
