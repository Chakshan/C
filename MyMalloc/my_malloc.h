#ifndef _MY_MALLOC_H_
#define _MY_MALLOC_H_

#include <stdlib.h>
#include <stdint.h>

void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);

typedef struct header {
	uint32_t id;
	size_t size;
	struct header *next;
	struct header *prev;
} header_t;

static header_t *find_optimal_block(size_t size);
static header_t *create_new_block(size_t size);
static size_t round_to_8(size_t x);

static void del(header_t *block);
static void insert_after(header_t *block, header_t *before_block);
static void insert_before(header_t *block, header_t *after_block);

#define DEFAULT_SBRK (2 << 15) // 32KB

#define ID 0xDEADBEEF

#define LOG_ENV_VAR "ENABLE_LOG"

#endif // _MY_MALLOC_H_
