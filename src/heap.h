#ifndef HEAP_H
#define HEAP_H

#define HEAP_ADDRESS 0x01000000

#define HEAP_SIZE_BYTES 104857600
#define HEAP_PYHS_PAGE_SIZE 4096

#define HEAP_PAGE_TAKEN 0x01
#define HEAP_PAGE_FREE 0x00

#define HEAP_PAGE_HAS_NEXT 0b10000000
#define HEAP_PAGE_IS_FIRST 0b01000000

struct heap
{

	void* saddr;
};
#endif
