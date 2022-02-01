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

#define LIST_INIT {.head = NULL}
#define LIST_ITER(list, ptr) \
	for (ptr = (list).head; ptr != NULL; ptr = ptr->next)


struct list_node {
	int val;
	struct list_node *next;
};

struct list {
	struct list_node *head;
};

void list_add(struct list *list, int val);
size_t list_size_get(const void *list);
void *list_copy(void *dest, const void *src, size_t size);
void list_destroy(void *list_ptr);

void list_add(struct list *list, int val) {
	struct list_node **ptr = &list->head;
	while(*ptr != NULL) {
		ptr = &(*ptr)->next;
	}
	*ptr = calloc(1, sizeof(struct list_node));
	**ptr = (struct list_node) {.val = val, .next = NULL};
}

size_t list_size_get(const void *list) {
	return sizeof(struct list);
}

void *list_copy(void *dest, const void *src, size_t size) {
	struct list *list_dest = dest;
	const struct list *list_src = src;
	struct list_node *ptr = list_src->head;
	while(ptr != NULL) {
		list_add(list_dest, ptr->val);
		ptr = ptr->next;
	}
	return dest;
}

void list_destroy(void *list_ptr) {
	struct list *list = list_ptr;
	struct list_node *ptr = list->head;
	while(ptr != NULL) {
		list->head = list->head->next;
		free(ptr);
		ptr = list->head;
	}
}


int main(void) {

	cmap_data_t key_interface = CREATE_INTERFACE(int_cmp, int_size_get);
	cmap_data_t val_interface = CREATE_INTERFACE4(NULL, list_size_get, list_copy, list_destroy);

	cmap_t map = cmap_init(&key_interface, &val_interface);

	struct list list = LIST_INIT;

	for (int i = 0; i < 10; i++)
		list_add(&list, i);

	int d[3] = {2, 1, 10};
	
	map.insert(&map, &d[0], &list);
	list_add(&list, 300);
	map.insert(&map, &d[1], &list);
	list_add(&list, -200);
	map.insert(&map, &d[2], &list);
	
	for (int i = 0; i < 3; i++) {
		struct list *list_ptr = (struct list *)map.search(&map, d + i);
		printf("the list of %d: ", d[i]);
		struct list_node *ptr = NULL;
		if (list_ptr) {
			LIST_ITER(*list_ptr, ptr) {
				printf("%d->", ptr->val);
			}
			printf("NULL\n");
		}
	}
	
	printf("Destroying...\n");
	map.destroy(&map);
	list_destroy(&list);
	printf("Finish\n");

	return 0;
}
