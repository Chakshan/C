#include <stdio.h>
#include <stdlib.h>
#include "sorted_list.h"
#include "test_sorted.h"
#include <stdarg.h>

sorted_list_t empty_list;
sorted_list_t short_list;
dl_node_t *node1, *node2, *node3;

int comparator(void *a, void *b) {
	return *(int *)(a) - *(int *)(b);
}

int main(void) {
	RUN_TEST(test_init);
	RUN_TEST(test_insert_empty);
	RUN_TEST(test_insert_middle);
	RUN_TEST(test_insert_middle_duplicate);
	RUN_TEST(test_insert_largest);
	RUN_TEST(test_insert_largest_duplicate);
	RUN_TEST(test_insert_smallest);
	RUN_TEST(test_insert_smallest_duplicate);
}

void test_dl_connections(int n, ...)
{
	va_list args;
	va_start(args, n);
	
	if (n < 1) {
		fprintf(stderr, "Invalid number of nodes\n");
		exit(1);
	} else if (n == 1) {
		dl_node_t *node = va_arg(args, dl_node_t *);
		TEST(node->prev == NULL);
		TEST(node->next == NULL);
	} else if (n == 2) {
		dl_node_t *node1 = va_arg(args, dl_node_t *);
		dl_node_t *node2 = va_arg(args, dl_node_t *);
		TEST(node1->prev == NULL);
		TEST(node1->next == node2);
		TEST(node2->prev == node1);
		TEST(node2->next == NULL);
	} else {
		dl_node_t *node1 = va_arg(args, dl_node_t *);
		dl_node_t *node2 = va_arg(args, dl_node_t *);
		TEST(node1->prev == NULL);
		TEST(node1->next == node2);
		dl_node_t *curr_node = node2;
		dl_node_t *prev_node = node1;
		dl_node_t *next_node; 
		for (int i = 2; i <= n - 1; i++) {
			next_node = va_arg(args, dl_node_t *);
			TEST(curr_node->prev == prev_node);
			TEST(curr_node->next == next_node);
			prev_node = curr_node;
			curr_node = next_node;
		}
		TEST(curr_node->prev == prev_node);
		TEST(curr_node->next == NULL);
	}
}

void test_dl_values(int n, ...)
{
	va_list args;
	va_start(args, n);
	for (int i = 0; i < n; i++) {
		dl_node_t *node = va_arg(args, dl_node_t *);
		int val = va_arg(args, int);
		TEST(*(int *)(node->data) == val);
	}
}

void setup() {
	empty_list.head = NULL;
	empty_list.tail = NULL;
	empty_list.size = 0;
	empty_list.comparator = comparator;

	node1 = (dl_node_t *)(malloc(sizeof(dl_node_t)));
	node2 = (dl_node_t *)(malloc(sizeof(dl_node_t)));
	node3 = (dl_node_t *)(malloc(sizeof(dl_node_t)));

	node1->data = malloc(sizeof(int));
	*(int *)(node1->data) = 1;
	node2->data = malloc(sizeof(int));
	*(int *)(node2->data) = 3;
	node3->data = malloc(sizeof(int));
	*(int *)(node3->data) = 5;

	node1->prev = NULL;
	node1->next = node2;
	node2->prev = node1;
	node2->next = node3;
	node3->prev = node2;
	node3->next = NULL;

	short_list.head = node1;
	short_list.tail = node3;
	short_list.size = 3;
	short_list.comparator = comparator;
}

void cleanup() {
	clear(&empty_list);
	clear(&short_list);
}

void test_init() {
	sorted_list_t list;
	init(&list, &comparator);
	TEST(list.size == 0);
	TEST(list.head == NULL);
	TEST(list.tail == NULL);
	TEST(list.comparator == comparator);
	
	int x = 4;
	int y = 2;
	TEST(list.comparator(&x, &y) > 0);
	
	x = 3;
	y = 3;	
	TEST(list.comparator(&x, &y) == 0);
	
	x = 2;
	y = 4;
	TEST(list.comparator(&x, &y) < 0);
}

void test_insert_empty() {
	int x = 1;
	insert(&empty_list, &x);
	TEST(empty_list.head != NULL);
	TEST(empty_list.head == empty_list.tail);
	TEST(empty_list.head->data == &x);
	TEST(empty_list.head->next == NULL);
	TEST(empty_list.head->prev == NULL);
}

void test_insert_middle() {
	int x = 2;
	insert(&short_list, &x);
	TEST(short_list.head == node1);
	TEST(short_list.tail == node3);
	test_dl_connections(4, node1, node1->next, node2, node3);
	test_dl_values(4, node1, 1, node1->next, 2, node2, 3, node3, 5);
	
	/*
	TEST(short_list.head == node1);
	TEST(node1->prev == NULL);
	TEST(node1->next->next == node2);
	TEST(node2->prev->prev = node1);
	TEST(node2->next == node3);
	TEST(node3->prev == node2);
	TEST(node3->next == NULL);

	TEST(*(int *)(node1->data) == 1);
	TEST(*(int *)(node1->next->data) == 2);
	TEST(*(int *)(node2->data) == 3);
	TEST(*(int *)(node3->data) == 5);
	*/
}

void test_insert_middle_duplicate() {
	int x = 3;
	insert(&short_list, &x);
	TEST(short_list.head == node1);
	TEST(short_list.tail == node3);
	test_dl_connections(4, node1, node1->next, node2, node3);
	test_dl_values(4, node1, 1, node1->next, 3, node2, 3, node3, 5);

	/*
	TEST(short_list.head == node1);
	TEST(node1->prev == NULL);
	TEST(node1->next->next == node2);
	TEST(node1->next == node2->prev);
	TEST(node2->prev->prev == node1);
	TEST(node2->next == node3);
	TEST(node3->prev == node2);
	TEST(node3->next == NULL);

	TEST(*(int *)(node1->data) == 1);
	TEST(*(int *)(node1->next->data) == 3);
	TEST(*(int *)(node2->data) == 3);
	TEST(*(int *)(node3->data) == 5);
	*/
}

void test_insert_largest() {
	int x = 6;
	insert(&short_list, &x);
	TEST(short_list.head == node1);
	test_dl_connections(4, node1, node2, node3, short_list.tail);
	test_dl_values(4, node1, 1, node2, 3, node3, 5, short_list.tail, 6);
	/*
	// check connections
	TEST(short_list.head == node1);
	TEST(node1->prev == NULL);
	TEST(node1->next == node2);
	TEST(node2->prev == node1);
	TEST(node2->next == node3);
	TEST(node3->prev == node2);
	TEST(node3->next == short_list.tail);
	TEST(short_list.tail->prev == node3);
	TEST(short_list.tail->next == NULL);

	// check values
	TEST(*(int *)(node1->data) == 1);
	TEST(*(int *)(node2->data) == 3);
	TEST(*(int *)(node3->data) == 5);
	TEST(*(int *)(short_list.tail->data) == 6);
	*/
}

void test_insert_largest_duplicate() {
	int x = 5;
	insert(&short_list, &x);
	TEST(short_list.head == node1);
	TEST(short_list.tail == node3);
	test_dl_connections(4, node1, node2, node2->next, node3);
	test_dl_values(4, node1, 1, node2, 3, node2->next, 5, node3, 5);
	/*
	TEST(short_list.head == node1);
	TEST(node1->prev == NULL);
	TEST(node1->next == node2);
	TEST(node2->prev == node1);
	TEST(node2->next->next == node3);
	TEST(node2->next == node3->prev);
	TEST(node3->prev->prev == node2);
	TEST(node3->next == NULL);

	TEST(*(int *)(node1->data) == 1);
	TEST(*(int *)(node2->data) == 3);
	TEST(*(int *)(node2->next->data) == 5);
	TEST(*(int *)(node3->data) == 5);
	*/
}

void test_insert_smallest() {
	int x = 0;
	insert(&short_list, &x);
	TEST(short_list.head == node1->prev);
	TEST(short_list.tail == node3);
	test_dl_connections(4, short_list.head, node1, node2, node3);
	test_dl_values(4, short_list.head, 0, node1, 1, node2, 3, node3, 5);
	/*
	TEST(short_list.head->prev == NULL);
	TEST(short_list.head->next == node1);
	TEST(node1->prev == short_list.head);
	TEST(node1->next == node2);
	TEST(node2->prev == node1);
	TEST(node2->next == node3);
	TEST(node3->prev == node2);
	TEST(node3->next == NULL);
	TEST(short_list.tail == node3);

	TEST(*(int *)(short_list.head->data) == 0);
	TEST(*(int *)(node1->data) == 1);
	TEST(*(int *)(node2->data) == 3);
	TEST(*(int *)(node3->data) == 5);
	*/
}

void test_insert_smallest_duplicate() {
	int x = 1;
	insert(&short_list, &x);
	TEST(short_list.head == node1->prev);
	TEST(short_list.tail == node3);
	test_dl_connections(4, short_list.head, node1, node2, node3);
	test_dl_values(4, short_list.head, 1, node1, 1, node2, 3, node3, 5);
}


