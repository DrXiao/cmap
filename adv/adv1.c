#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmap.h"

int int_cmp(const void *d1, const void *d2) {
	const int *int1 = d1, *int2 = d2;
	return *int1 - *int2;
}

size_t int_size_get(const void *d1) {
	return sizeof(int);
}

size_t str_size_get(const void *str) {
	return strlen(str) + 1;	
}

int main(void) {

	cmap_data_t key_interface = CREATE_INTERFACE(int_cmp, int_size_get);
	cmap_data_t val_interface = CREATE_INTERFACE(NULL, str_size_get);

	cmap_t map = cmap_init(&key_interface, &val_interface);

	int d[3] = {2, 1, 10};

	map.insert(&map, &d[0], "String");
	map.insert(&map, &d[1], "Function parameters");
	map.insert(&map, &d[2], "C programming");

	map.insert(&map, &d[2], "If you don't feel that C language is easy, it is just that you don't realize how complicated the life is.");

	for (int i = 0; i < 3; i++) {
		char *str = (char *)map.search(&map, d + i);
		printf("the str of %d: %s\n", d[i], str);
	}
	
	printf("Destroying...\n");
	map.destroy(&map);
	printf("Finish\n");

	return 0;
}
