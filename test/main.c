#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmap.h"

int cmp(const void *key1, const void *key2) {
	return strcmp(key1, key2);
}

size_t key_size_get(const void *key) {
	return strlen(key) + 1;
}

size_t val_size_get(const void *val) {
	return sizeof(int);
}

int main(void) {

	cmap_t map = CMAP_INIT(cmp, key_size_get, val_size_get);
	cmap_root_init(&map);
	
	int val = 10;
	map.insert(&map, "Hello", &val);
	val = 20;
	map.insert(&map, "World", &val);
	printf("%d\n", *(int *)map.search(&map, "World"));

	map.destroy(&map);
	
	cmap_t *alloc_map = cmap_alloc(cmp, key_size_get, val_size_get);

	printf("%p\n", alloc_map->search(alloc_map, "Hello"));
	alloc_map->destroy(alloc_map);
	free(alloc_map);

	return 0;
}
