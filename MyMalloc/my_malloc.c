#include "my_malloc.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

// head and tail of free list
static header_t *head;
static header_t *tail;

static bool debug = false;
static char log_buf[100];

// Constructor for library
// check if logging enabled and initialize accordingly
void __attribute__ ((constructor)) __init__(void)
{
	printf("MYMALLOC library is loaded.\n");
	char *env_var = getenv(LOG_ENV_VAR);
	if (env_var && strcmp(env_var, "1") == 0) {
		debug = true;
		printf("Logging enabled.\n");
	}
}

static void log_message(char* format, ...)
{
	if (!debug) return;
	va_list args;
	va_start(args, format);
	vsnprintf(log_buf, sizeof(log_buf), format, args);
	va_end(args);
	puts(log_buf);
}

void *malloc(size_t size)
{
	if (size <= 0) {
		return NULL;
	}

	header_t *optimal_block = find_optimal_block(size);
	// if there no free block available, make one with sbrk()
	if (optimal_block == NULL) {
		header_t *new_block = create_new_block(size);
		
		if (tail) {
			insert_after(new_block, tail);
		} else {
			new_block->next = NULL;
			new_block->prev = NULL;
			head = new_block;
			tail = new_block;
		}

		optimal_block = new_block;
	}

	void *ptr = optimal_block + 1;

	// carve out from optimal block

	// case 1: entire block must be used	
	if ((optimal_block->size - size) <= sizeof(header_t)) {
		del(optimal_block);
	} 

	// case 2: partial block is used; insert remainder back into free list 
	else {
		header_t *cut_block = (void *)(optimal_block + 1) + size;
		cut_block->id = ID;
		cut_block->size = optimal_block->size - size - sizeof(header_t);
		optimal_block->size = size;

		insert_after(cut_block, optimal_block);
		del(optimal_block);
	}

	log_message("Allocated block at %p of size %lu bytes", optimal_block, optimal_block->size);

	return ptr;
}

// Finds the smallest block that can contain the size of the allocation
// Returns NULL if no such block
static header_t *find_optimal_block(size_t size)
{
	header_t *optimal_block = NULL;
	header_t *curr_node = head;	
	while (curr_node) {
		if (curr_node->size == size) {
			return curr_node;
		}
		if (curr_node->size > size) {
			if (optimal_block == NULL || curr_node->size < optimal_block->size) {
				optimal_block = curr_node;
			}
		} 
		curr_node = curr_node->next;
	}

	return optimal_block;
}

// Expands heap using sbrk() to create a new block
// Returns NULL in case of failure
static header_t *create_new_block(size_t size)
{
	size_t aligned_size = round_to_8(size + sizeof(header_t));
	size_t block_size = (DEFAULT_SBRK > aligned_size) ? DEFAULT_SBRK : aligned_size;
	header_t *new_block = sbrk(block_size);
	log_message("Expanding heap by %lu bytes.\n", block_size);

	if (new_block == (void *)(-1)) { 	// sbrk() failure
		log_message("SBRK failure.");
		return NULL;
	}

	new_block->id = ID;	
	new_block->size = block_size - sizeof(header_t);

	log_message("Created new free block at %p of size %lu bytes.\n", new_block, new_block->size);
	return new_block;
}

static size_t round_to_8(size_t x)
{
	if (x & 0x7) {
		return (x / 8 + 1) * 8;
	}
	return x; 
}


void free(void *ptr)
{
	// check if ptr is valid
	if (ptr < (void *)sizeof(header_t)) {
		return;
	}

	header_t *block = (void *)(ptr) - sizeof(header_t);

	// check if block is valid
	if (block == NULL || block->id != ID) {	
		return;
	}

	// insert block into free list in order of memory address
	if (head == NULL) {
		block->next = NULL;
		block->prev = NULL;
		head = block;
		tail = block;
		return;
	}
	if (block > tail) {
		insert_after(block, tail);
	} else if (block < head) {
		insert_before(block, head);
	} else {
		header_t *after = head;
		while (after < block) {
			after = after->next;	
		}
		// handle freeing an already freed block
		if (after == block) {
			return;
		}
		insert_before(block, after);
	}

	log_message("Freed block at %p of size %lu bytes", block, block->size);

	// Merge adjancent blocks if they are also free
	if (((void *)(block + 1) + block->size) == block->next) {
		block->size += (sizeof(header_t) + block->next->size);
		del(block->next);
		log_message("Merged with next.");
	}
	if (block->prev && ((void *)(block - 1) - block->prev->size) == block->prev) {
		block->prev->size += (sizeof(header_t) + block->size);
		del(block);
		log_message("Merged with prev.");
	}
}

void *calloc(size_t nmemb, size_t size)
{
	void *ptr = malloc(nmemb * size);
	
	if (ptr == NULL) {
		return NULL;
	}
	
	memset(ptr, 0, nmemb * size);
	return ptr;
}

static void del(header_t *block)
{
	if (block == NULL) {
		log_message("Trying to delete NULL block.");
		return;
	}

	if (block == head) {
		head = block->next;
	} else {
		block->prev->next = block->next;
	}

	if (block == tail) {
		tail = block->prev;
	} else {
		block->next->prev = block->prev;
	}
}

static void insert_after(header_t *block, header_t *before_block)
{
	if (block == NULL || before_block == NULL) {
		log_message("Trying to insert NULL or after NULL.");
		return;
	}

	if (block == before_block) {
		log_message("Trying to create self referential node in free list.\n");
		return;
	}

	block->prev = before_block;
	block->next = before_block->next;

	if (before_block->next) {
		before_block->next->prev = block;
	}

	before_block->next = block;

	if (before_block == tail) {
		tail = block;
	}
}

static void insert_before(header_t *block, header_t *after_block)
{
	if (block == NULL || after_block == NULL) {
		log_message("Trying to insert NULL or before NULL");
		return;
	}

	if (block == after_block) {
		log_message("Trying to create self referential node in free list.\n");
		return;
	}

	block->next = after_block;
	block->prev = after_block->prev;
	
	if (after_block->prev) {
		after_block->prev->next = block;
	}

	after_block->prev = block;

	if (after_block == head) {
		head = block;
	}
}
