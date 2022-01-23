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

// For testing.
struct tuple {
	char key[1024];
	int val;
} tuples[] = {{"Hello", 10},
			  {"Linux", 20},
			  {"World", 30},
			  {"Everything is a file", 40},
			  {"Seamphore", -100},
			  {"Mutex", 40},
			  {"Spinlock", (int)1e9},
			  {"Arch Linux", 404},
			  {"Ubuntu Linux", 200},
			  {"Debian Linux", 204},
			  {"AAA", 30},
			  {"BBB", 50}};

int main(void) {

	cmap_t map = CMAP_INIT(cmp, key_size_get, val_size_get);

	char buf[1024];

	for (int i = 0; i < sizeof(tuples) / sizeof(struct tuple); i++) {
		printf("Insert (%s, %d)\n", tuples[i].key, tuples[i].val);
		map.insert(&map, tuples[i].key, &tuples[i].val);
	}

	printf("Searching...\n");
	while (fgets(buf, sizeof(buf), stdin) != NULL) {
		buf[strlen(buf) - 1] = '\0';
		int *accesser = map.search(&map, buf);
		printf("Key: %s, ", buf);
		if (accesser) {
			printf("Get: %d, delete the node\n", *accesser);
			map.erase(&map, buf);
		}
		else
			printf("Not exist: %p\n", accesser);
	}

	map.destroy(&map);
	printf("Finish\n");
	return 0;
}
