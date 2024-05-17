#include "test_hashmap.h"
#include "hashmap.h"

#define CAPACITY 3

hashmap_t map;
entry_t *table;
int num;

entry_t *exp_table;

int hash(void *num) 
{
	return (int)(num);
}

int compare(void *a, void *b)
{
	return (int)(a) - (int)(b);
}

int main(void)
{
	RUN_TEST(test_get_no_collision);
	RUN_TEST(test_get_collision);
	RUN_TEST(test_get_collision_gap);
	RUN_TEST(test_get_nonexistant);

	RUN_TEST(test_put_no_collision);
	RUN_TEST(test_put_collision);
	RUN_TEST(test_put_duplicate);
	RUN_TEST(test_put_full);

	RUN_TEST(test_del_no_collision);
	RUN_TEST(test_del_collision);
	RUN_TEST(test_del_collision_gap);
	RUN_TEST(test_del_nonexistant);
	
}

void setup()
{
	init(&map, CAPACITY, hash, compare);
	table = map.hash_table;

	exp_table = (entry_t *)calloc(CAPACITY, sizeof(entry_t));
}

void cleanup()
{
	destroy(&map);
	free(exp_table);
}

void set_entry(entry_t *entry, int key, int val, bool del)
{
	entry->key = (void *) key;
	entry->value = (void *) val;
	entry->deleted = del;
}

void test_table()
{
	for (int i = 0; i < CAPACITY; i++) {
		entry_t actual = table[i];
		entry_t expect = exp_table[i];
		TEST(actual.key == expect.key);
		TEST(actual.value == expect.value);
		TEST(actual.deleted == expect.deleted);
	}
}

void test_get_no_collision()
{
	set_entry(&table[0], 3, 13, false);
	set_entry(&table[1], 1, 11, false);
	set_entry(&table[2], 2, 12, false);

	void *res = get(&map, (void *) 1);
	TEST((int)res == 11);
}

void test_get_collision()
{
	set_entry(&table[0], 3, 13, false);
	set_entry(&table[1], 6, 16, false);
	set_entry(&table[2], 9, 19, false);

	void *res = get(&map, (void *) 6);
	TEST((int)res == 16);
}

void test_get_wrap()
{
	set_entry(&table[0], 1, 11, false);
	set_entry(&table[1], 4, 14, false);
	set_entry(&table[2], 7, 17, false);

	void *res = get(&map, (void *) 1);
	TEST((int)res == 11);
}

void test_get_collision_gap()
{
	set_entry(&table[0], 3, 13, false);
	set_entry(&table[1], (int) NULL, (int) NULL, true);
	set_entry(&table[2], 6, 16, false);

	void *res = get(&map, (void *) 6);
	TEST((int)res == 16);
}

void test_get_nonexistant()
{
	set_entry(&table[0], (int) NULL, (int) NULL, false);
	set_entry(&table[1], 4, 14, false);
	set_entry(&table[2], 5, 15, false);

	void *res = get(&map, (void *) 7);
	TEST(res == (int) NULL);
}

void test_put_no_collision()
{
	set_entry(&table[0], 3, 13, false);
	set_entry(&table[2], 2, 12, false);

	int res = put(&map, (void *) 1, (void *) 11);

	set_entry(&exp_table[0], 3, 13, false);
	set_entry(&exp_table[1], 1, 11, false);
	set_entry(&exp_table[2], 2, 12, false);

	test_table();	
	TEST(res == 0);
}

void test_put_collision() 
{
	set_entry(&table[0], 3, 13, false);
	set_entry(&table[2], 5, 15, false);

	int res = put(&map, (void *) 6, (void *) 16);

	set_entry(&exp_table[0], 3, 13, false);
	set_entry(&exp_table[1], 6, 16, false);
	set_entry(&exp_table[2], 5, 15, false);

	test_table();
	TEST(res == 0);	
}

void test_put_duplicate() 
{
	set_entry(&table[0], 3, 13, false);
	set_entry(&table[1], 1, 11, false);
	set_entry(&table[2], 2, 12, false);

	int res = put(&map, (void *) 1, (void *) 100);

	set_entry(&exp_table[0], 3, 13, false);
	set_entry(&exp_table[1], 1, 100, false);
	set_entry(&exp_table[2], 2, 12, false);

	test_table();
	TEST(res == 0);	
}

void test_put_full()
{
	set_entry(&table[0], 3, 13, false);
	set_entry(&table[1], 1, 11, false);
	set_entry(&table[2], 2, 12, false);
	
	int res = put(&map, (void *) 4, (void *) 14);

	set_entry(&exp_table[0], 3, 13, false);
	set_entry(&exp_table[1], 1, 11, false);
	set_entry(&exp_table[2], 2, 12, false);

	test_table();
	TEST(res == -1);
}

void test_del_no_collision()
{
	set_entry(&table[0], 3, 13, false);
	set_entry(&table[1], 1, 11, false);
	set_entry(&table[2], 2, 12, false);

	void *deleted = del(&map, (void *)1);

	set_entry(&exp_table[0], 3, 13, false);
	set_entry(&exp_table[1], (int) NULL, (int) NULL, true);
	set_entry(&exp_table[2], 2, 12, false);

	test_table();
	TEST((int)deleted == 11);
}

void test_del_collision()
{
	set_entry(&table[0], 3, 13, false);
	set_entry(&table[1], 6, 16, false);
	set_entry(&table[2], 9, 19, false);

	void *deleted = del(&map, (void *) 9);

	set_entry(&exp_table[0], 3, 13, false);
	set_entry(&exp_table[1], 6, 16, false);
	set_entry(&exp_table[2], (int) NULL, (int) NULL, true);
	
	test_table();
	TEST((int)deleted == 19);
}

void test_del_collision_gap()
{
	set_entry(&table[0], 3, 13, false);
	set_entry(&table[1], (int) NULL, (int) NULL, true);
	set_entry(&table[2], 6, 16, false);

	void *deleted = del(&map, (void *) 6);

	set_entry(&exp_table[0], 3, 13, false);
	set_entry(&exp_table[1], (int) NULL, (int) NULL, true);
	set_entry(&exp_table[2], (int) NULL, (int) NULL, true);

	test_table();
	TEST((int)deleted == 16);
}

void test_del_nonexistant()
{
	set_entry(&table[0], 3, 13, false);
	set_entry(&table[1], 1, 11, false);
	set_entry(&table[2], 2, 12, false);

	void *deleted = del(&map, (void *) 5);

	set_entry(&exp_table[0], 3, 13, false);
	set_entry(&exp_table[1], 1, 11, false);
	set_entry(&exp_table[2], 2, 12, false);
	
	test_table();
	TEST(deleted == (int) NULL);
}

