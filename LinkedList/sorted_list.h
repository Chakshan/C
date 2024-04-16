#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include "node.h"
#include <stdbool.h>

typedef struct {
	dl_node_t *head;
	dl_node_t *tail;
	int size;
	int (*comparator)(void *, void *);
} sorted_list_t;

void init(sorted_list_t *list, int (*comparator)(void *, void*));
void *front(sorted_list_t *list);
void *back(sorted_list_t *list);
void insert(sorted_list_t *list, void *elem);
void pop_front(sorted_list_t *list);
void pop_back(sorted_list_t *list);

bool empty(sorted_list_t *list);
void clear(sorted_list_t *list);


#endif