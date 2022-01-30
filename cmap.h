#ifndef __C_MAP__
#define __C_MAP__
#include <stdlib.h>
#include <stdbool.h>
#define CREATE_INTERFACE(cmp_func, size_get_func)                              \
	{.data = NULL,                                                         \
	 .cmp = cmp_func,                                                      \
	 .data_size_get = size_get_func,                                       \
	 .copy = memcpy,                                                       \
	 .destroy = free};

typedef struct cmap cmap_t;
typedef struct cmap_data cmap_data_t;
typedef struct cmap_node cmap_node_t;

struct cmap_data {
	void *data;
	int (*const cmp)(const void *, const void *);
	size_t (*const data_size_get)(const void *);
	void *(*const copy)(void *, const void *, size_t);
	void (*const destroy)(void *);
};

/**
 * struct cmap_node - the information of a node used by cmap.
 * @black: 	color of a node. (either red or black)
 * @key:	key of a node.
 * @val:	value of a node.
 * @parent:	pointer to parent of the node.
 * @left:	pointer to left subtree of the node.
 * @right	pointer to right subtree of the node.
 *
 * A simple definition of a cmap node to imitate <map> container in C++.
 */
struct cmap_node {
	bool black;
	cmap_data_t key;
	cmap_data_t val;
	cmap_node_t *parent, *left, *right;

	int (*const cmp)(const void *, const void *);
	void (*const insert_key)(cmap_node_t *, const void *);
	void (*const insert_val)(cmap_node_t *, const void *);
	void (*const destroy)(cmap_node_t *);
};

struct cmap {
	cmap_node_t *root;
	void *(*const search)(cmap_t *, const void *);
	void (*const insert)(cmap_t *, const void *, const void *);
	bool (*const erase)(cmap_t *, const void *);
	void (*const destroy)(cmap_t *);

	cmap_data_t key_interface;
	void (*const insert_key)(cmap_node_t *, const void *);

	cmap_data_t val_interface;
	void (*const insert_val)(cmap_node_t *, const void *);
};

cmap_t cmap_init(cmap_data_t *, cmap_data_t *);

void *cmap_search(cmap_t *, const void *);

void cmap_insert(cmap_t *, const void *, const void *);

bool cmap_erase(cmap_t *, const void *);

void cmap_destroy(cmap_t *);

void *cmap_alloc(cmap_data_t *, cmap_data_t *);

#endif
