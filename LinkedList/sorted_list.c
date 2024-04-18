#include "sorted_list.h"
#include <stdlib.h>

// Initialize empty list
void init(sorted_list_t *list, int (*comparator)(void *, void *)) {
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	list->comparator = comparator;
}

// Return pointer to first element
void *front(sorted_list_t *list) {
	return list->head->data;
}

// Return pointer to last element
void  *back(sorted_list_t *list) {
	return list->tail->data;
}

void insert(sorted_list_t *list, void *elem) {
	dl_node_t *new_node = (dl_node_t *) malloc(sizeof(dl_node_t));
	new_node->data = elem;

	if (empty(list)) {
		new_node->next = NULL;
		new_node->prev = NULL;
		list->head = new_node;
		list->tail = new_node;
	} else {
		dl_node_t *curr_node = list->head;
		// find node with value equal or greater
		while (curr_node && list->comparator(elem, curr_node->data) > 0) {
			curr_node = curr_node->next;
		}

		if (curr_node) {
			// inserted new head
			if (curr_node == list->head) { 
				new_node->next = curr_node;
				curr_node->prev = new_node;
				new_node->prev = NULL;
				list->head = new_node;
			} 
			// inserted middle element
			else { 
				new_node->next = curr_node;
				new_node->prev = curr_node->prev;
				curr_node->prev->next = new_node;
				curr_node->prev = new_node;
			}
		} else { // inserting new tail
			list->tail->next = new_node;
			new_node->prev = list->tail;
			new_node->next = NULL;
			list->tail = new_node;
		}
	}
	list->size++;
}

// Remove first element
void pop_front(sorted_list_t *list) {
	if (empty(list)) return;
	if (list->size == 1) {
		free(list->head);
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
		return;
	}
	dl_node_t *to_remove = list->head; 
	list->head = list->head->next;
	list->head->prev = NULL;
	list->size--;
	free(to_remove);
}

// Remove last element
void pop_back(sorted_list_t *list) {
	if (empty(list)) return;
	if (list->size == 1) {
		free(list->tail);
		list->head = NULL;
		list->tail = NULL;
		list->size = 0;
		return;
	}
	dl_node_t *to_remove = list->tail;
	list->tail = list->tail->prev;
	list->tail->next = NULL;
	list->size--;
	free(to_remove);
}

// True if empty; false if not
bool empty(sorted_list_t *list) {
	return (list->size == 0);
}

// Remove all elements in the list
void clear(sorted_list_t *list) {
	if (empty(list)) return;

	// free all elements
	dl_node_t *curr_node = list->head;
	dl_node_t *to_remove;
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