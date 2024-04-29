#include "hashmap.h"
#include <stdlib.h>
#include <stdio.h>

void init(hashmap_t *map, int (*hash)(void*)) {
	entry_t *hash_table = (entry_t *) calloc(CAPACITY, sizeof(entry_t));
	map->hash_table = hash_table;
	map->size = 0;
	map->hash = hash;

	for (int i = 0; i < CAPACITY; i++) {
		map->hash_table[i].key = NULL;
		map->hash_table[i].deleted = false;
	}
}

void *get(hashmap_t *map, void *key) {
	int hash = map->hash(key) % CAPACITY;

	for (int i = hash; ; i = (i + 1) % CAPACITY) {
		entry_t curr_entry = map->hash_table[i];
		if (curr_entry.key == key) 
			return curr_entry.value;
		if (curr_entry.key == NULL && !curr_entry.deleted)
			return NULL;	
	}
	return NULL;
}

void put(hashmap_t *map, void *key, void *value) {
	if (map->size == CAPACITY) {
		fprintf(stderr, "Max capacity reached; cannot put");
		return;
	}

	int hash = map->hash(key) % CAPACITY;
	for (int i = hash; ; i = (i + 1) % CAPACITY) {
		entry_t curr_entry = map->hash_table[i];
		if (curr_entry.key == NULL) {
			curr_entry.key = key;
			curr_entry.value = value;
			curr_entry.deleted = false;
		}
	}
}

void *del(hashmap_t *map, void *key) {
	int hash = map->hash(key) % CAPACITY;

	for (int i = hash; ; i = (i + 1) % CAPACITY) {
		entry_t curr_entry = map->hash_table[i];
		if (curr_entry.key == key) {
			curr_entry.key == NULL;
			void *val = curr_entry.value;
			curr_entry.value = NULL;
			return val;
		}
		if (curr_entry.key == NULL && !curr_entry.deleted)
			return NULL;	
	}
	return NULL;
}

void destroy(hashmap_t *map) {
	for (int i = 0; i < CAPACITY; i++) {
		free(map->hash_table + i);
	}
	free(map);
}