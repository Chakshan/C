#include "test_my_malloc.h"

int main()
{
	RUN_TEST(test_first_malloc);
}


void test_first_malloc()
{
	int *ptr = (int *)my_malloc(sizeof(int));
	assert(ptr != NULL);
	*ptr = 5;
	assert(*ptr == 5);

	int arr_len = 8;

	int *arr = (int *)my_malloc(sizeof(int) * arr_len);
	assert(ptr != NULL);
	
	for (int i = 0; i < arr_len; i++) {
		arr[i] = i;
	}

	for (int i = 0; i < arr_len; i++) {
		assert(arr[i] == i);
	}

	my_free(ptr);
	my_free(arr);

	int *ptr2 = (int *)my_malloc(sizeof(int));
	*ptr2 = (2 << 8) - 1;
	assert(*ptr == (2 << 8) - 1);
}

