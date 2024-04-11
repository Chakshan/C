#include "linked_list.h"
#include <stdlib.h>

// Initialize empty list
void init(linked_list_t *list) {
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

// Return pointer to first element
void* front(linked_list_t *list) {
	return list->head->data;
}

// Return pointer to last element
void* back(linked_list_t *list) {
	return list->tail->data;
}

// Add element to the front
void push_front(linked_list_t *list, void *elem) {
	node_t *new_node = (node_t *) malloc(sizeof(node_t));
	new_node->data = elem;
	new_node->prev = NULL;
	
	if (empty(list)) { // adding first element
		new_node->next = NULL;
		list->head = new_node;
		list->tail = new_node;
	} else {
		new_node->next = list->head;
		list->head->prev = new_node;
		list->head = new_node;
	}
	list->size++;
}

// Add element to the back
void push_back(linked_list_t *list, void *elem) {
	node_t *new_node = (node_t *) malloc(sizeof(node_t));
	new_node->data = elem;
	new_node->next = NULL;

	if (empty(list)) { // adding first element
		new_node->prev = NULL;
		list->head = new_node;
		list->tail = new_node;
	} else {
		list->tail->next = new_node;
		new_node->prev = list->tail;
		list->tail = new_node;
	}
	list->size++;
}

// Remove first element
void pop_front(linked_list_t *list) {
	if (empty(list)) return;
	if (list->size == 1) {
		free(list->head);
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
		return;
	}
	node_t *to_remove = list->head; 
	list->head = list->head->next;
	list->head->prev = NULL;
	list->size--;
	free(to_remove);
}

// Remove last element
void pop_back(linked_list_t *list) {
	if (empty(list)) return;
	if (list->size == 1) {
		free(list->tail);
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
		return;
	}
	node_t *to_remove = list->tail;
	list->tail = list->tail->prev;
	list->tail->next = NULL;
	list->size--;
	free(to_remove);
}

// True if empty; false if not
bool empty(linked_list_t *list) {
	return (list->size == 0);
}

// Remove all elements in the list
void clear(linked_list_t *list) {
	if (empty(list)) return;

	// free all elements
	node_t *curr_node = list->head;
	node_t *to_remove;
	while (curr_node->next != NULL) {
		to_remove = curr_node;
		curr_node = curr_node->next;
		free(to_remove);
	}
	free(curr_node);

	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}