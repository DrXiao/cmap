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
	cmap_t map = CMAP_INIT(cmp, key_size_get, val_size_get);

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

	map.destroy(&map);
	printf("Finish\n");	
	return 0;
}
