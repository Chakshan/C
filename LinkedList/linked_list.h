#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include "node.h"

typedef struct {
	dl_node_t *head;
	dl_node_t *tail;
	int size;
} linked_list_t;


void init(linked_list_t *list); // Initialize empty list

void *front(linked_list_t *list); // Return pointer to first element
void *back(linked_list_t *list); // Return pointer to last element

void push_front(linked_list_t *list, void *elem); // Add element to the front
void push_back(linked_list_t *list, void *elem); // Add element to the back
void pop_front(linked_list_t *list); // Remove first element
void pop_back(linked_list_t *list); // Remove last element

bool empty(linked_list_t *list); // True if empty; false if not
void clear(linked_list_t *list); // Removes all elements in the list

#endif