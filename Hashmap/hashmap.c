#include "hashmap.h"
#include <stdlib.h>
#include <stdio.h>

void init(hashmap_t *map, int capacity, int (*hash)(void*)) {
	if (capacity <= 0)
		capacity = DEFAULT_CAPACITY;

	entry_t *hash_table = (entry_t *) calloc(capacity, sizeof(entry_t));
	if (hash_table == NULL) {
		fprintf(stderr, "Unable to allocate memory for hash table\n");
		return;
	}
	map->hash_table = hash_table;
	map->capacity;
	map->size = 0;
	map->hash = hash;

	for (int i = 0; i < capacity; i++) {
		map->hash_table[i].key = NULL;
		map->hash_table[i].deleted = false;
	}
}

void *get(hashmap_t *map, void *key) {
	int hash = map->hash(key) % map->capacity;
	int i = hash;
	entry_t curr_entry;
	do {
		curr_entry = map->hash_table[i];
		if (curr_entry.key == key) 
			return curr_entry.value;
		if (curr_entry.key == NULL && !curr_entry.deleted)
			return NULL;	
		i = (i + 1) % map->capacity;
	} while (i != hash);

	return NULL;
}

int put(hashmap_t *map, void *key, void *value) {
	if (map->size == map->capacity) {
		fprintf(stderr, "Max capacity reached; cannot put");
		return -1;
	}

	int hash = map->hash(key) % map->capacity;
	int i = hash;
	entry_t curr_entry;
	do {
		curr_entry = map->hash_table[i];
		if (curr_entry.key == NULL) {
			curr_entry.key = key;
			curr_entry.value = value;
			curr_entry.deleted = false;
		}
		i = (i + 1) % map->capacity;
	} while (i != hash);

	return 0;
}

void *del(hashmap_t *map, void *key) {
	int hash = map->hash(key) % map->capacity;
	int i = hash;
	entry_t curr_entry;

	do {
		curr_entry = map->hash_table[i];
		if (curr_entry.key == key) {
			curr_entry.key == NULL;
			void *val = curr_entry.value;
			curr_entry.value = NULL;
			return val;
		}
		if (curr_entry.key == NULL && !curr_entry.deleted)
			return NULL;	
		i = (i + 1) % map->capacity;	
	} while (i != hash);
	
	return NULL;
}

void destroy(hashmap_t *map) {
	free(map->hash_table);
	free(map);
}