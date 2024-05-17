#ifndef _TEST_HASHMAP_H_
#define _TEST_HASHMAP_H_

#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"

void test_get_no_collision();
void test_get_collision();
void test_get_collision_gap();
void test_get_wrap();
void test_get_nonexistant();

void test_put_no_collision();
void test_put_collision();
void test_put_duplicate();
void test_put_full();

void test_del_no_collision();
void test_del_collision();
void test_del_collision_gap();
void test_del_nonexistant();

void setup();
void cleanup();

void set_entry(entry_t *entry, int key, int val, bool del);
void test_table();

#define TEST(x)  if (!(x)) { printf("test failed"); exit(1); }
#define RUN_TEST(x) printf("Running: " #x); setup(); x(); cleanup(); printf("...passed\n")

#endif // _TEST_HASHMAP_H_