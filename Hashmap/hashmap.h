#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include <stdbool.h>

#define CAPACITY 11

typedef struct {
    void *key;
    void *value;
    bool deleted;
} entry_t;

typedef struct {
    entry_t *hash_table;
    int size;
    int (*hash)(void *);
} hashmap_t;

void init(hashmap_t *map, int (*hash)(void*));
void *get(hashmap_t *map, void *key);
void put(hashmap_t *map, void *key, void *value);
void *del(hashmap_t *map, void *key);
void destroy(hashmap_t *map);

#endif // _HASHMAP_H_