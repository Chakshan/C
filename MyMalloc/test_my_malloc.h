#ifndef _TEST_MY_MALLOC_H_
#define _TEST_MY_MALLOC_H_


/*
Features to test:
 - malloc
	 - memory is accessible and multiple allocations are disjoint
	 - zero allocation is null
	 - large allocations & expand heap when no free blocks
	 - smallest is used block
 - free
	 - merging with next
	 - merging with prev
	 - merging with both
	 - freeing a block that is already freed
	 - freeing an incorrect address
	 - 
*/

void test_valid_memory();
void test_zero_allocation();
void test_large_allocation();
void test_smallest_block_used();
void test_merge_next();
void test_merge_prev();
void test_merge_both();
void test_free_already_freed();
void test_free_invalid_ptr();

#endif // _TEST_MY_MALLOC_H_