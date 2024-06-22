#include "my_malloc.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// head and tail of free list
static block_t *head;
static block_t *tail;

static void log(char* format, ...)
{
	char* env_var = getenv(LOG_ENV_VAR);
	if (env_var && env_var[0] == '1' && env_var[1] == '\0') {
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\n");
	}
}

void *malloc(size_t size)
{
	if (size <= 0) {
		return NULL;
	}

	// find smallest block that can accomodate the allocation
	block_t *optimal_block = head;
	block_t *curr_node = head;	
	while (curr_node) {
		if (curr_node->size >= size && (curr_node->size - size) < (optimal_block->size - size)) {
			optimal_block = curr_node;
		}
		curr_node = curr_node->next;
	}

	// if there no free block available, make one with sbrk()
	if (optimal_block == NULL || optimal_block->size < size) {
		size_t aligned_size = ((size + sizeof(block_t)) / 8 + 1) * 8; // align to 8 bytes
		size_t block_size = (DEFAULT_SBRK > aligned_size) ? DEFAULT_SBRK : aligned_size;
		block_t *new_block = sbrk(block_size);
		log("Expanding heap by %lu bytes.", block_size);

		if (new_block == (void *)(-1)) { 	// sbrk() failure
			log("SBRK failure.");
			return NULL;
		}

		new_block->id = ID;	
		new_block->size = block_size - sizeof(block_t);
		
		if (tail) {
			insert_after(new_block, tail);
		} else {
			new_block->next = NULL;
			new_block->prev = NULL;
			head = new_block;
			tail = new_block;
		}

		log("Created free block of size %lu bytes.", new_block->size);

		optimal_block = new_block;
	}

	void *ptr = optimal_block + 1;

	// carve out from optimal block

	// case 1: entire block must be used	
	if ((optimal_block->size - size) <= sizeof(block_t)) {
		del(optimal_block);
	} 

	// case 2: partial block is used; insert remainder back into free list 
	else {
		block_t *cut_block  = (void *)(optimal_block + 1) + size;
		cut_block->id = ID;
		cut_block->size = optimal_block->size - size - sizeof(block_t);
		optimal_block->size = size;

		insert_after(cut_block, optimal_block);
		del(optimal_block);

		log("Free block of size %lu bytes inserted into free list.", cut_block->size);
	}

	return ptr;
}


void free(void *ptr)
{
	block_t *block = (void *)(ptr) - sizeof(block_t);

	// check if block is valid
	if (block == NULL || block->id != ID) {	
		return;
	}

	// insert block into free list in order of
	// memory address
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
		block_t *after = head;
		while (after < block) {
			after = after->next;	
		}
		// handle freeing an already freed block
		if (after == block) {
			return;
		}
		insert_before(block, after);
	}

	// Merge adjancent blocks if they are also free
	if (((void *)(block + 1) + block->size) == block->next) {
		block->size += (sizeof(block_t) + block->next->size);
		del(block->next);
		log("Merged with next.");
	}
	if (block->prev && ((void *)(block - 1) - block->prev->size) == block->prev) {
		block->prev->size += (sizeof(block_t) + block->size);
		del(block);
		log("Merged with prev.");
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

static void del(block_t *block)
{
	if (block == NULL) {
		log("Trying to delete NULL block.");
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

static void insert_after(block_t *block, block_t *before_block)
{
	if (block == NULL || before_block == NULL) {
		log("Trying to insert NULL or after NULL.");
		return;
	}

	if (block == before_block) {
		log("Trying to create self referential node in free list.\n");
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

static void insert_before(block_t *block, block_t *after_block)
{
	if (block == NULL || after_block == NULL) {
		log("Trying to insert NULL or before NULL");
		return;
	}

	if (block == after_block) {
		log("Trying to create self referential node in free list.\n");
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
