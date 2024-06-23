#include "test_my_malloc.h"
#include "my_malloc.h"

// #define ABORT_AFTER_FAILURE
#include "../Test/test.h"

int main()
{
	RUN_TEST(test_valid_memory);
	RUN_TEST(test_zero_allocation);
	RUN_TEST(test_large_allocation);
	RUN_TEST(test_smallest_block_used);
	RUN_TEST(test_merge_next);
	RUN_TEST(test_merge_prev);
	RUN_TEST(test_merge_both);
	RUN_TEST(test_free_already_freed);
	RUN_TEST(test_free_invalid_ptr);
}

static void setup() {}
static void cleanup() {}

void test_valid_memory()
{
	int num1 = 50;
	int num2 = 25;
	int *ptr1 = (int *)malloc(num1 * sizeof(int));
	int *ptr2 = (int *)malloc(num2 * sizeof(int));

	for (int i = 0; i < num1; i++) {
		ptr1[i] = i;
	}

	for (int j = 0; j < num2; j++) {
		ptr2[j] = j;
	}

	ASSERT((((void *)ptr1 + 200) < (void *)ptr2) || (((void *)ptr2 + 100) < (void *)ptr1));
}

void test_zero_allocation()
{
	void *ptr = malloc(0);
	ASSERT(ptr == NULL);
}

void test_large_allocation()
{
	int *ptr = (int *)malloc(DEFAULT_SBRK);
	ptr[0] = 0;
	ptr[DEFAULT_SBRK / sizeof(int) - 1] = 3000; 

	int *ptr2 = (int *)malloc(DEFAULT_SBRK + 10000);
	ptr2[0] = 0;
	ptr[(DEFAULT_SBRK + 10000) / sizeof(int) - 1] = 9000;

	free(ptr);
	free(ptr2);
}

void test_smallest_block_used()
{
	int *ptr0 = (int *)malloc(sizeof(int));
	int *ptr1 = (int *)malloc(8 * sizeof(int));
	int *ptr2 = (int *)malloc(sizeof(int));
	int *ptr3 = (int *)malloc(4 * sizeof(int));
	int *ptr4 = (int *)malloc(sizeof(int));
	int *ptr5 = (int *)malloc(13 * sizeof(int));
	int *ptr6 = (int *)malloc(sizeof(int));

	free(ptr1);
	free(ptr3);
	free(ptr5);
	
	int *ptrx = (int *)malloc(3 * sizeof(int));
	ASSERT(ptrx == ptr3);
	int *ptry = (int *)malloc(5 * sizeof(int));
	ASSERT(ptry == ptr1);

	free(ptr0);

	free(ptr2);
	free(ptr4);
	free(ptr6);
	free(ptrx);
	free(ptry);
}

void test_merge_next() {
	void *ptr0 = malloc(4);
	int *ptr1 = malloc(2 * sizeof(int));
	int *ptr2 = malloc(4 * sizeof(int));
	void *ptr3 = malloc(4);

	free(ptr2);
	free(ptr1);

	int *ptr = malloc(5 * sizeof(int));
	ASSERT(ptr == ptr1);

	free(ptr0);
	free(ptr3);
	free(ptr);
}

void test_merge_prev() {
	void *ptr0 = malloc(4);
	int *ptr1 = malloc(2 * sizeof(int));
	int *ptr2 = malloc(4 * sizeof(int));
	void *ptr3 = malloc(4);

	free(ptr1);
	free(ptr2);

	int *ptr = malloc(5 * sizeof(int));
	ASSERT(ptr == ptr1);

	free(ptr0);
	free(ptr3);
	free(ptr);
}

void test_merge_both() {
	void *ptr0 = malloc(4);
	int *ptr1 = malloc(2 * sizeof(int));
	int *ptr2 = malloc(3 * sizeof(int));
	int *ptr3 = malloc(4 * sizeof(int));
	void *ptr4 = malloc(4);

	free(ptr1);
	free(ptr3);
	free(ptr2);

	int *ptr = malloc(8 * sizeof(int));
	ASSERT(ptr == ptr1);

	free(ptr0);
	free(ptr4);
	free(ptr);
}

void test_free_already_freed()
{
	void *ptr0 = malloc(30);
	void *ptr1 = malloc(10);
	void *ptr2 = malloc(40);
	void *ptr3 = malloc(20);
	void *ptr4 = malloc(50);

	free(ptr1);
	free(ptr3);
	
	free(ptr1);

	// should exhibit normal behavior
	void *ptrx = malloc(16);
	ASSERT(ptrx == ptr3);

	free(ptr0);
	free(ptr4);
}

void test_free_invalid_ptr()
{
	void *ptr0 = malloc(30);
	void *ptr1 = malloc(10);
	void *ptr2 = malloc(40);
	void *ptr3 = malloc(20);
	void *ptr4 = malloc(50);

	free(ptr1);
	free(ptr3);
	
	free(ptr1 + 4);

	// should exhibit normal behavior
	void *ptrx = malloc(16);
	ASSERT(ptrx == ptr3);

	free(ptr0);
	free(ptr4);
}