#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define FRAME_SIZE 1024
#define FIRST_FRAME_ADDRESS 0x01000000
#define FRAME_PHYSICAL_SIZE_BYTE 104857600
#define FRAME_TABLE_ADDRESS 0x00007E00

#define FRAME_IS_TAKEN 0x01
#define FRAME_IS_FREE 0x00

#define FRAME_HAS_NEXT 0b10000000
#define FRAME_IS_FIRST 0b01000000

typedef uint16_t FRAME_TABLE_ENTRY;
struct frame_table
{
	FRAME_TABLE_ENTRY* entries;
	size_t total;	
};

struct frames
{
	struct frame_table* table;

	void* saddr;
};

int kernel_frames_create(struct frames* frames, void* first_frame, void* last_frame, struct frame_table* table);
void* kernel_frame_alloc(struct frames* frames, size_t size);
void kernel_frame_free(struct frames* frames, void* ptr);

#endif
