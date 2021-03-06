#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
} tuples[100];

int main(void) {
	
	FILE *input_node = fopen("test/test.in", "r");
	if (input_node == NULL)
		input_node = stdin;
	srand(time(NULL));
	cmap_data_t key_interface = CREATE_INTERFACE(cmp, key_size_get);
	cmap_data_t val_interface = CREATE_INTERFACE(NULL, val_size_get);

	cmap_t map = cmap_init(&key_interface, &val_interface);

	char buf[1024];
	int i = 0;
	while(fgets(tuples[i].key, sizeof(tuples[i].key), input_node) != NULL) {
		tuples[i].key[strlen(tuples[i].key) - 1] = '\0';
		tuples[i].val = rand() % 1000;
		printf("Insert (%s, %d)\n", tuples[i].key, tuples[i].val);
		map.insert(&map, tuples[i].key, &tuples[i].val);
		i++;
	}
	fclose(input_node);

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
