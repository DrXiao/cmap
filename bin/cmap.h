#ifndef __C_MAP__
#define __C_MAP__
#include <stdlib.h>
#include <stdbool.h>
#define CREATE_INTERFACE(cmp_func, size_get_func)                              \
	{.data = NULL,                                                         \
	 .cmp = cmp_func,                                                      \
	 .data_size_get = size_get_func,                                       \
	 .copy = memcpy,                                                       \
	 .destroy = NULL,                                                      \
	 .dealloc = free}

#define CREATE_INTERFACE4(cmp_func, size_get_func, copy_func, destroy_func)    \
	{.data = NULL,                                                         \
	 .cmp = cmp_func,                                                      \
	 .data_size_get = size_get_func,                                       \
	 .copy = copy_func,                                                    \
	 .destroy = destroy_func,                                              \
	 .dealloc = free}

typedef struct cmap cmap_t;
typedef struct cmap_node cmap_node_t;
typedef struct cmap_data cmap_data_t;

struct cmap_data {
	void *data;
	int (*const cmp)(const void *, const void *);
	size_t (*const data_size_get)(const void *);
	void *(*const copy)(void *, const void *, size_t);
	void (*const destroy)(void *);
	void (*const dealloc)(void *);
};

struct cmap {
	cmap_node_t *root;
	cmap_data_t key_interface;
	cmap_data_t val_interface;
	void *(*const search)(cmap_t *, const void *);
	void (*const insert)(cmap_t *, const void *, const void *);
	bool (*const erase)(cmap_t *, const void *);
	void (*const destroy)(cmap_t *);
	void (*const dealloc)(void *);
};

cmap_t cmap_init(cmap_data_t *, cmap_data_t *);

void *cmap_alloc(cmap_data_t *, cmap_data_t *);

#endif
