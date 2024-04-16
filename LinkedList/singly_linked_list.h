#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include <stdbool.h>
#include "node.h"

typedef struct {
	sl_node_t *head;
	sl_node_t *tail;
	int size;
} s_linked_list_t;

void init(s_linked_list_t *list);
void init(s_linked_list_t *list); // Initialize empty list

void *front(s_linked_list_t *list); // Return pointer to first element
void *back(s_linked_list_t *list); // Return pointer to last element

void push_front(s_linked_list_t *list, void *elem); // Add element to the front
void push_back(s_linked_list_t *list, void *elem); // Add element to the back
void pop_front(s_linked_list_t *list); // Remove first element
void pop_back(s_linked_list_t *list); // Remove last element

bool empty(s_linked_list_t *list); // True if empty; false if not
void clear(s_linked_list_t *list); // Removes all elements in the list
#endif