#ifndef _MY_MALLOC_H_
#define _MY_MALLOC_H_

#include <stdlib.h>

void *my_malloc(size_t size);
void free(void * ptr);

typedef struct free_block {
	uint32_t id;
	size_t size;
	free_block *next;
	free_block *prev;
} free_block_t;

#define DEFAULT_SBRK 2 << 15 // 32KB

#define ID 0xDEADBEEF

#endif // _MY_MALLOC_H_