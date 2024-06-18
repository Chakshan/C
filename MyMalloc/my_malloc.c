#include "my_malloc.h"
#include <unistd.h>

free_block_t *head;
free_block_t *tail;

void *my_malloc(size_t size)
{
	// find smallest block that can accomodate the allocation
	free_block_t *optimal_block = head;
	free_block_t *curr_node = head;	
	while (curr_node) {
		if (curr_node->size >= size && (curr_node->size - size) < (optimal_block->size - size)) {
			optimal_block = curr_node;
		}
		curr_node = curr_node->next;
	}

	// if there is no free block available, get more memory with sbrk()
	if (optimal_block == NULL) {
		size_t aligned_size = ((size + sizeof(free_block_t)) / 8 + 1) * 8; // align to 8 bytes
		size_t block_size = max(DEFAULT_SBRK, aligned_size);
		free_block_t *new_block = sbrk(block_size);

		if (new_block == -1) { 	// sbrk() failure
			return NULL;
		}

		new_block->id = ID;	
		new_block->size = block_size - sizeof(free_block_t);
		new_block->next = NULL;
		new_block->prev = tail;
		tail = new_block;

		if (head == NULL) {
			head = new_block;
		}

		optimal_block = new_block;
	}

	void *ptr = optimal_block + 1;

	// carve out from optimal block

	// case 1: entire block must be used; delete node from free list	
	if ((optimal_block->size - size) <= sizeof(free_block_t)) {
		optimal_block->prev->next = optimal_block->next;
		optimal_block->next->prev = optimal_block->prev;
	} 

	// case 2: partial block is used; cut new block at start of unused portion 
	else {
		free_block_t *cut_block  = (void *)(optimal_block) + size;
		cut_block->id = ID;
		cut_block->next = optimal_block->next;
		cut_block->prev = optimal_block->prev;
		cut_block->size = optimal_block->size - size - sizeof(free_block_t);

		optimal_block->prev->next = cut_block;
		optimal_block->next->prev = cut_block;
	}

	return ptr;
}


void free(void *ptr)
{
	free_block_t *block = ptr;

	// check if block is valid
	if (head == NULL || block == NULL || block->id != ID) {	
		return;
	}

	// insert block into free list
	if (block > tail) {
		tail->next = block;
		block->prev = tail;
		block->next = NULL;
		tail = block;
	} else if (block < head) {
		head->prev = block;
		block->next = head;
		block->prev = NULL;
		head = block;	
	} else {
		free_block_t *after = head;
		while (after < block) {
			after = after->next;	
		}
		after->prev->next = block;
		after->prev = block;
	}

	// add merging
}