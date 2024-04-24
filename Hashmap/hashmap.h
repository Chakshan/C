#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include <stdbool.h>

typedef struct {
    void *key;
    void *value;
} entry_t;

typedef struct {
    entry_t *hash_table;
    int size;
    int (*hash)(void *);
} hashmap_t;

void init(hashmap_t *map, int min_capacity, int (*hash)(void*));
void insert(hashmap_t *map, void *key, void *value);
void *remove(hashmap_t *map, void *key);
void *get(hashmap_t *map, void *key);
void clear(hashmap_t *map);

#endif
