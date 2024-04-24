#ifndef _HASHMAP_H_
#define _HASHMAP_H_

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
void *get(hashmap_t *map, void *key);
void put(hashmap_t *map, void *key, void *value);
void *del(hashmap_t *map, void *key);
void destroy(hashmap_t *map);

#endif // _HASHMAP_H_