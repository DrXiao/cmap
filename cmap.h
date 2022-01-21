#ifndef __C_MAP__
#define __C_MAP__
#include <stdbool.h>
#define CMAP_INIT(cmp, key_size_get, val_size_get)                             \
    {                                                                          \
        .root = NULL, .cmp = cmp, .key_size_get = key_size_get,                \
        .val_size_get = val_size_get, .search = cmap_search,                   \
        .insert = cmap_insert, .erase = cmap_erase, .destroy = cmap_destroy    \
    }

typedef struct cmap cmap_t;

struct cmap {
    struct cmap_node *root;
    int (*cmp)(const void *, const void *);
    size_t (*key_size_get)(const void *);
    size_t (*val_size_get)(const void *);
    void *(*const search)(cmap_t *, const void *);
    void (*const insert)(cmap_t *, const void *, const void *);
    bool (*const erase)(cmap_t *, const void *);
    void (*destroy)(cmap_t *);
};

void cmap_root_init(cmap_t *);

void *cmap_search(cmap_t *, const void *);

void cmap_insert(cmap_t *, const void *, const void *);

bool cmap_erase(cmap_t *, const void *);

void cmap_destroy(cmap_t *);

void *cmap_alloc(int (*)(const void *, const void *),
    			 size_t (*)(const void *),
    			 size_t (*)(const void *));

void cmap_validate(cmap_t *);

#endif
