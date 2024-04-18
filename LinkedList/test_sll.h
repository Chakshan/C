#ifndef _TEST_SLL_H_
#define _TEST_SLL_H_

void setup();
void cleanup();

void test_init();

void test_front_empty();
void test_front_nonempty();

void test_back_empty();
void test_back_nonempty();

void test_push_front_empty();
void test_push_front_nonempty();

void test_push_back_empty();
void test_push_back_nonempty();

void test_pop_front_empty();
void test_pop_front_nonempty();

void test_pop_back_empty();
void test_pop_back_nonempty();

#define TEST(x)  if (!(x)) { printf("test failed"); exit(1); }
#define RUN_TEST(x) printf("Running: " #x); setup(); x(); cleanup(); printf("...passed\n")

#endif // _TEST_SLL_H_