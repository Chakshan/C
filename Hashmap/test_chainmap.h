#ifndef _TEST_CHAINMAP_H_
#define _TEST_CHAINMAP_H_

void setup();

void cleanup();

void test_get();

void test_put_overwrite();

void test_del();

#define RUN_TEST(x) printf("Running: " #x); setup(); x(); cleanup(); printf("...passed\n")

#endif // _TEST_CHAINMAP_H_