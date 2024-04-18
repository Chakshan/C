#ifndef _TEST_SORTED_H_
#define _TEST_SORTED_H_

void setup();
void cleanup();

void test_init();
void test_insert_empty();

void test_insert_middle();
void test_insert_middle_duplicate();

void test_insert_largest();
void test_insert_largest_duplicate();

void test_insert_smallest();
void test_insert_smallest_duplicate();

void test_dl_connections(int n, ...);
void test_dl_values(int n, ...);

#define TEST(x)  if (!(x)) { printf("test failed"); exit(1); }
#define RUN_TEST(x) printf("Running: " #x " ..."); setup(); x(); cleanup(); printf("passed\n")

#endif // _TEST_SORTED_H_
