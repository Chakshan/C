#include "singly_linked_list.h"
#include <stdlib.h>

// Initialize empty list
void init(s_linked_list_t *list) {
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
}

// Return pointer to first element
void *front(s_linked_list_t *list) {
	if (empty(list)) return NULL;
	return list->head->data;
}

// Return pointer to last element
void  *back(s_linked_list_t *list) {
	if (empty(list)) return NULL;
	return list->tail->data;
}

// Add element to the front
void push_front(s_linked_list_t *list, void *elem) {
	sl_node_t *new_node = (sl_node_t *) malloc(sizeof(sl_node_t));
	new_node->data = elem;
	
	if (empty(list)) { // adding first element
		new_node->next = NULL;
		list->head = new_node;
		list->tail = new_node;
	} else {
		new_node->next = list->head;
		list->head = new_node;
	}
	list->size++;
}

// Add element to the back
void push_back(s_linked_list_t *list, void *elem) {
	sl_node_t *new_node = (sl_node_t *) malloc(sizeof(sl_node_t));
	new_node->data = elem;
	new_node->next = NULL;

	if (empty(list)) { // adding first element
		list->head = new_node;
		list->tail = new_node;
	} else {
		list->tail->next = new_node;
		list->tail = new_node;
	}
	list->size++;
}

// Remove first element
void pop_front(s_linked_list_t *list) {
	if (empty(list)) return;
	if (list->size == 1) {
		free(list->head);
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
		return;
	}
	sl_node_t *to_remove = list->head; 
	list->head = list->head->next;
	list->size--;
	free(to_remove);
}

// Remove last element
void pop_back(s_linked_list_t *list) {
	if (empty(list)) return;
	if (list->size == 1) {
		free(list->tail);
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
		return;
	}
	sl_node_t *curr_node = list->head;
	while (curr_node->next != list->tail) {
		curr_node = curr_node->next;
	}
	free(list->tail);
	list->tail = curr_node;
	list->tail->next = NULL;
	list->size--;
}

// True if empty; false if not
bool empty(s_linked_list_t *list) {
	return (list->size == 0);
}

// Remove all elements in the list
void clear(s_linked_list_t *list) {
	if (empty(list)) return;

	// free all elements
	sl_node_t *curr_node = list->head;
	sl_node_t *to_remove;
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