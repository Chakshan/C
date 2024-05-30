#ifndef CHAINMAP_H 
#define CHAINMAP_H

#define DEFAULT_CHAINS 3

typedef struct hashnode {
    void *key;
    void *value;
	struct hashnode *next;
} hashnode_t;

typedef struct {
    hashnode_t **hash_table;
    int chains;
    int size;
    int (*hash)(void *);
    int (*compare)(void *, void *);
} chainmap_t;

void init(chainmap_t *map, int chains, 
          int (*hash)(void*), int (*compare)(void *, void *));
void *get(const chainmap_t *map, void *key);
int put(chainmap_t *map, void *key, void *value);
void *del(chainmap_t *map, void *key);
void destroy(chainmap_t *map);

#endif // CHAINMAP_H