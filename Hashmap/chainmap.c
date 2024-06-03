#include "chainmap.h"
#include <stdlib.h>
#include <stdio.h>

void init(chainmap_t *map, int chains, 
		  int (*hash)(void*), int (*compare)(void *, void *))
{
	if (chains <= 0)
		chains = DEFAULT_CHAINS;
	
	hashnode_t **hash_table = (hashnode_t **) calloc(chains, sizeof(hashnode_t *));
	if (hash_table == NULL) {
		fprintf(stderr, "Unable to allocate memory for hash table\n");
		return;
	}
	map->hash_table = hash_table;
	map->chains = chains;
	map->size = 0;
	map->hash = hash;
	map->compare = compare;
}

void *get(const chainmap_t *map, void *key)
{
	int index = map->hash(key) % map->chains;
	hashnode_t *curr_node = map->hash_table[index];

	while(curr_node) {
		if (map->compare(curr_node->key, key) == 0) {
			return curr_node->value;
		}
		curr_node = curr_node->next;
	}
	return NULL;
}

int put(chainmap_t *map, void *key, void *value)
{
	hashnode_t *new_node = (hashnode_t *) malloc(sizeof(hashnode_t));
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;

	int index = map->hash(key) % map->chains;
	hashnode_t *curr_node = map->hash_table[index];

	if (curr_node == NULL) {
		map->hash_table[index] = new_node;
		map->size++;
		return 0;
	}

	while (curr_node->next) {
		if (map->compare(curr_node->key, key) == 0) {
			curr_node->value = value;
			free(new_node);
			return 0;
		}
		curr_node = curr_node->next;
	}

	if (map->compare(curr_node->key, key)) {
		curr_node->next = new_node;
		map->size++;
	} else {
		curr_node->value = value;
		free(new_node);
	}

	return 0;
}
void *del(chainmap_t *map, void *key)
{
	int index = map->hash(key) % map->chains;
	hashnode_t *curr_node = map->hash_table[index];
	hashnode_t *prev_node = NULL;

	while (curr_node) {
		if (map->compare(curr_node->key, key) == 0) {
			if (prev_node)
				prev_node->next = curr_node->next;
			else
				map->hash_table[index] = curr_node->next;
			void *val = curr_node->value;
			free(curr_node);
			map->size--;
			return val;
		}
		prev_node = curr_node;
		curr_node = curr_node->next;
	}

	return NULL;
}

void destroy(chainmap_t *map)
{
	for (int i = 0; i < map->chains; i++) {
		hashnode_t *curr_node = map->hash_table[i];
		while (curr_node) {
			hashnode_t *temp = curr_node;
			curr_node = curr_node->next;
			free(temp);
		}
	}
	free(map->hash_table);
}