#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "test_chainmap.h"
#include "chainmap.h"

chainmap_t map;
int hash(void *num)
{
	unsigned long address = (unsigned long) num;
	return (int) address;
}
int compare(void *a, void *b)
{
	return (int)((unsigned long)(a) - (unsigned long)(b));
}

/* wrapper test (t) functions for convenience */
int tput(unsigned long key, unsigned long val) 
{
	return put(&map, (void *) key, (void *) val);
}

unsigned long tget(unsigned long key) 
{
	return (unsigned long) get(&map, (void *) key);
}

unsigned long tdel(unsigned long key) 
{
	return (unsigned long) del(&map, (void *) key);
}

int main(void) 
{
	RUN_TEST(test_get);
	RUN_TEST(test_put_overwrite);
	RUN_TEST(test_del);

	printf("All tests passed :)\n");
}

void setup() 
{
	// map
	// =========
	// 0: 3, 6, 9
	// 1: 1
	// 2:

	init(&map, 3, hash, compare);
	tput(3, 3);
	tput(6, 6);
	tput(9, 9);
	tput(1, 1);
}

void cleanup()
{
	destroy(&map);
}

void test_get()
{
	assert(tget(3) == 3);
	assert(tget(6) == 6);
	assert(tget(9) == 9);
	assert(tget(1) == 1);

	// test get nonexistent
	assert(tget(2) == 0); // NULL
}

void test_put_overwrite()
{
	tput(3, 30);
	tput(6, 60);
	tput(1, 10);
	assert(tget(3) == 30);
	assert(tget(6) == 60);
	assert(tget(1) == 10);
}

void test_del()
{
	assert(tdel(3) == 3);
	assert(tget(3) == 0); // NULL
	assert(tdel(9) == 9);
	assert(tget(9) == 0); // NULL
	assert(tdel(1) == 1);
	assert(tget(1) == 0); // NULL;

	// test del nonexistent
	assert(tget(2) == 0); // 
}


