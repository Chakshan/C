#ifndef _NODE_H_
#define _NODE_H_

typedef struct dl_node {
	void *data;
	struct dl_node *next;
	struct dl_node *prev;
} dl_node_t;

typedef struct sl_node {
	void *data;
	struct sl_node *next;
} sl_node_t;

#endif // _NODE_H_
