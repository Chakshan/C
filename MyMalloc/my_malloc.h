#ifndef _MY_MALLOC_H_
#define _MY_MALLOC_H_

#include <stdlib.h>

void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmenb, size_t size);

typedef struct block {
	uint32_t id;
	size_t size;
	struct block *next;
	struct block *prev;
} block_t;

static void del(block_t *block);
static void insert_after(block_t *block, block_t *before_block);
static void insert_before(block_t *block, block_t *after_block);

#define DEFAULT_SBRK (2 << 15) // 32KB

#define ID 0xDEADBEEF

#define LOG_ENV_VAR "ENABLE_LOG"

#endif // _MY_MALLOC_H_