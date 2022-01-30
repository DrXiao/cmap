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

	cmap_data_t key_interface = CREATE_INTERFACE(cmp, key_size_get);
	cmap_data_t val_interface = CREATE_INTERFACE(NULL, val_size_get);

	cmap_t map = cmap_init(&key_interface, &val_interface);

	int val = 10;
	map.insert(&map, "Hello", &val);
	val = 20;
	map.insert(&map, "World", &val);
	printf("%d\n", *(int *)map.search(&map, "World"));

	map.destroy(&map);

	cmap_t *alloc_map = cmap_alloc(&key_interface, &val_interface);
	
	printf("%p\n", alloc_map->search(alloc_map, "Hello"));
	
	alloc_map->destroy(alloc_map);
	free(alloc_map);

	return 0;
}
