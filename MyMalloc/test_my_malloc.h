#ifndef _TEST_MY_MALLOC_H_
#define _TEST_MY_MALLOC_H_

#include "my_malloc.h"
#include "assert.h"
#include <stdio.h>
#include <stdlib.h>

void test_basic_allocation();
void test_zero_allocation();
void test_multiple_allocation();
void test_large_allocation();
void test_free_reallocate();
void test_memory_overlap();


#define RUN_TEST(x) \
	printf("Running: " #x); \
	x(); \
	printf("...passed\n")

#endif // _TEST_MY_MALLOC_H_