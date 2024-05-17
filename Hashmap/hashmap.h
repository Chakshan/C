#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include <stdbool.h>

#define DEFAULT_CAPACITY 11

typedef struct {
    void *key;
    void *value;
    bool deleted;
} entry_t;

typedef struct {
    entry_t *hash_table;
    int capacity;
    int size;
    int (*hash)(void *);
    int (*compare)(void *, void *);
} hashmap_t;

void init(hashmap_t *map, int capacity, int (*hash)(void*), int (*compare)(void *, void *));
void *get(const hashmap_t *map, void *key);
int put(hashmap_t *map, void *key, void *value);
void *del(hashmap_t *map, void *key);
void destroy(hashmap_t *map);

#endif // _HASHMAP_H_