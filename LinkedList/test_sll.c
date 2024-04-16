#include <stdlib.h>
#include <stdio.h>
#include "singly_linked_list.h"
#include "test_sll.h"

s_linked_list_t empty_list;
s_linked_list_t short_list;

sl_node_t *node1, *node2, *node3;

int main(void) {
	RUN_TEST(test_init);
	RUN_TEST(test_front_empty);
	RUN_TEST(test_front_nonempty);
	RUN_TEST(test_back_empty);
	RUN_TEST(test_back_nonempty);
	RUN_TEST(test_push_front_empty);
	RUN_TEST(test_push_front_nonempty);
	RUN_TEST(test_pop_back_empty);
	RUN_TEST(test_push_back_nonempty);
	RUN_TEST(test_pop_front_empty);
	RUN_TEST(test_pop_front_nonempty);
	RUN_TEST(test_pop_back_empty);
	RUN_TEST(test_pop_back_nonempty);
}

void setup() {
	empty_list.head = NULL;
	empty_list.tail = NULL;
	empty_list.size = 0;

	node1 = (sl_node_t *)(malloc(sizeof(sl_node_t)));
	node2 = (sl_node_t *)(malloc(sizeof(sl_node_t)));
	node3 = (sl_node_t *)(malloc(sizeof(sl_node_t)));

	node1->data = malloc(sizeof(int));
	*(int *)(node1->data) = 1;
	node2->data = malloc(sizeof(int));
	*(int *)(node2->data) = 2;
	node3->data = malloc(sizeof(int));
	*(int *)(node3->data) = 3;

	node1->next = node2;
	node2->next = node3;
	node3->next = NULL;

	short_list.head = node1;
	short_list.tail = node3;
	short_list.size = 3;
}

void cleanup() {
	clear(&empty_list);
	clear(&short_list);
}

void test_init() {
	s_linked_list_t list;
	init(&list);
	TEST(list.size == 0);
	TEST(list.head == NULL);
	TEST(list.tail == NULL);
}

void test_front_empty() {
	TEST(front(&empty_list) == NULL);
}

void test_front_nonempty() {
	TEST(*(int *)(front(&short_list)) == 1);
}

void test_back_empty() {
	TEST(back(&empty_list) == NULL);
}

void test_back_nonempty() {
	TEST(*(int *)(back(&short_list)) == 3);
}

void test_push_front_empty() {
	int x = 1;
	push_front(&empty_list, &x);
	TEST(empty_list.head != NULL);
	TEST(empty_list.head == empty_list.tail);
	TEST(empty_list.head->data == &x);
	TEST(empty_list.head->next == NULL);
}

void test_push_front_nonempty() {
	int x = 0;
	push_front(&short_list, &x);

	// check connections
	TEST(short_list.head->next == node1);
	TEST(node1->next == node2);
	TEST(node2->next == node3);
	TEST(node3->next == NULL);
	TEST(short_list.tail == node3);

	// check values
	TEST(*(int *)(short_list.head->data) == 0);
	TEST(*(int *)(node1->data) == 1);
	TEST(*(int *)(node2->data) == 2);
	TEST(*(int *)(node3->data) == 3);
}

void test_push_back_empty() {
	int x = 1;
	push_back(&empty_list, &x);
	TEST(empty_list.head != NULL);
	TEST(empty_list.head == empty_list.tail);
	TEST(empty_list.head->data == &x);
	TEST(empty_list.head->next == NULL);
}

void test_push_back_nonempty() {
	int x = 4;
	push_back(&short_list, &x);

	// check connections
	TEST(short_list.head == node1);
	TEST(node1->next == node2);
	TEST(node2->next == node3);
	TEST(node3->next == short_list.tail);
	TEST(short_list.tail->next == NULL);

	// check values
	TEST(*(int *)(node1->data) == 1);
	TEST(*(int *)(node2->data) == 2);
	TEST(*(int *)(node3->data) == 3);
	TEST(*(int *)(short_list.tail->data) == 4);
}

void test_pop_front_empty() {
	pop_front(&empty_list);
	TEST(empty_list.size == 0);
	TEST(empty_list.head == NULL);
	TEST(empty_list.tail == NULL);
}

void test_pop_front_nonempty() {
	pop_front(&short_list);
	TEST(short_list.head = node2);
	TEST(node2->next == node3);
	TEST(node3->next == NULL);
	TEST(short_list.tail == node3);

	TEST(*(int *)(node2->data) == 2);
	TEST(*(int *)(node3->data) == 3);
}

void test_pop_back_empty() {
	pop_back(&empty_list);
	TEST(empty_list.size == 0);
	TEST(empty_list.head == NULL);
	TEST(empty_list.tail == NULL);
}

void test_pop_back_nonempty() {
	pop_back(&short_list);
	TEST(short_list.head == node1);
	TEST(node1->next == node2);
	TEST(node2->next == NULL);
	TEST(short_list.tail == node2);

	TEST(*(int *)(node1->data) == 1);
	TEST(*(int *)(node2->data) == 2);
}