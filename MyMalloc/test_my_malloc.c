#include "test_my_malloc.h"

int main()
{
	RUN_TEST(test_basic_allocation);
	RUN_TEST(test_zero_allocation);
	RUN_TEST(test_multiple_allocation);
	RUN_TEST(test_large_allocation);
	RUN_TEST(test_free_reallocate);
	RUN_TEST(test_memory_overlap);
}


void test_basic_allocation()
{
	int *ptr = (int *)my_malloc(sizeof(int));
	*ptr = 5;
	my_free(ptr);
}

void test_zero_allocation()
{
	void *ptr = my_malloc(0);
	assert(ptr != NULL);
	my_free(ptr);
}


void test_multiple_allocation()
{
	int *ptr1 = (int *)my_malloc(sizeof(int) * 100);
	int *ptr2 = (int *)my_malloc(sizeof(int) * 200);
	int *ptr3 = (int *)my_malloc(sizeof(int) * 300);
	
	ptr1[0] = 0;
	ptr1[99] = 99;
	ptr2[0] = 0;
	ptr2[199] = 199;
	ptr3[0] = 0;
	ptr3[299] = 299;

	my_free(ptr1);
	my_free(ptr2);
	my_free(ptr3);
}

void test_large_allocation()
{
	int *ptr = (int *)my_malloc(DEFAULT_SBRK);
	ptr[0] = 0;
	ptr[DEFAULT_SBRK / sizeof(int) - 1] = 3000; 

	int *ptr2 = (int *)my_malloc(DEFAULT_SBRK + 10000);
	ptr2[0] = 0;
	ptr[(DEFAULT_SBRK + 10000) / sizeof(int) - 1] = 9000;

	my_free(ptr);
	my_free(ptr2);
}

void test_free_reallocate()
{
	void *ptr1 = my_malloc(200);
	assert(ptr1 != NULL);
	
	my_free(ptr1);
	void *ptr2 = my_malloc(200);
	assert(ptr2 == ptr1);

	my_free(ptr2);
}

void test_memory_overlap()
{
	void *ptr1 = my_malloc(200);
	void *ptr2 = my_malloc(100);

	assert(ptr1 != NULL);
	assert(ptr2 != NULL);
	assert((ptr1 + 200 < ptr2) || (ptr2 + 100 < ptr1));
}
