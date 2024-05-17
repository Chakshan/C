#include "hashmap.h"
#include <stdlib.h>
#include <stdio.h>

void init(hashmap_t *map, int capacity, int (*hash)(void*), int (*compare)(void *, void *)) 
{
	if (capacity <= 0)
		capacity = DEFAULT_CAPACITY;

	entry_t *hash_table = (entry_t *) calloc(capacity, sizeof(entry_t));
	if (hash_table == NULL) {
		fprintf(stderr, "Unable to allocate memory for hash table\n");
		return;
	}
	map->hash_table = hash_table;
	map->capacity = capacity;
	map->size = 0;
	map->hash = hash;
	map->compare = compare;
}

void *get(const hashmap_t *map, void *key) 
{
	int hash = map->hash(key) % map->capacity;
	int i = hash;
	entry_t curr_entry;
	do {
		curr_entry = map->hash_table[i];
		if (map->compare(curr_entry.key, key) == 0) 
			return curr_entry.value;
		if (curr_entry.key == NULL && !curr_entry.deleted)
			return NULL;	
		i = (i + 1) % map->capacity;
	} while (i != hash);

	return NULL;
}

int put(hashmap_t *map, void *key, void *value) 
{
	int hash = map->hash(key) % map->capacity;
	int i = hash;
	entry_t *curr_entry;

	// update entry if key exists
	do {
		curr_entry = map->hash_table + i;
		if (map->compare(curr_entry->key, key) == 0) {
			curr_entry->value = value;
			return 0;
		}
		if (curr_entry->key == NULL && !curr_entry->deleted)
			break;
		i = (i + 1) % map->capacity;
	} while (i != hash);
	
	
	i = hash;
	do {
		curr_entry = map->hash_table + i;
		if (curr_entry->key == NULL) {
			curr_entry->key = key;
			curr_entry->value = value;
			curr_entry->deleted = false;
			return 0;
		}
		i = (i + 1) % map->capacity;
	} while (i != hash);

	fprintf(stderr, "Max capacity reached; cannot put");
	return -1;
}

void *del(hashmap_t *map, void *key) 
{
	int hash = map->hash(key) % map->capacity;
	int i = hash;
	entry_t *curr_entry;

	do {
		curr_entry = map->hash_table + i;
		if (curr_entry->key == key) {
			curr_entry->key = NULL;
			void *val = curr_entry->value;
			curr_entry->value = NULL;
			curr_entry->deleted = true;
			return val;
		}
		if (curr_entry->key == NULL && !curr_entry->deleted)
			return NULL;	
		i = (i + 1) % map->capacity;	
	} while (i != hash);
	
	return NULL;
}

void destroy(hashmap_t *map) 
{
	free(map->hash_table);
}