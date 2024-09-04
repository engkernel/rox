#ifndef FRAME_H
#define FRAME_H

#define FRAME_SIZE 1024
#define FIRST_FRAME_ADDRESS 0x01000000
#define FRAME_PHYSCIAL_SIZE_BYTE 104857600
#define FRAME_TABLE_ADDRESS 0x00007E00

#define FRAME_IS_TAKEN 0x01
#define FRAME_IS_FREE 0x00

#define FRAME_HAS_NEXT 0b10000000
#define FRAME_IS_FIRST 0b01000000

typedef uint16_t FRAME_TABLE_ENTRY;
struct frame_tables
{
	FRAME_TABLE_ENTRY* entries;
	size_t total;	
};

struct frames
{
	struct frame_tables* table;

	void* saddr;
};

#endif
