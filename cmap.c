#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "cmap.h"
#define DEBUG 1
#if DEBUG == 1
#include <stdio.h>
#endif

/*
 * C map - 	An associative array implemented by C language.
 *		In C++, it is called 'map', so this implementation
 *		is called as 'C map' because it is written by C.
 *
 *		This is also implemented by Red-Black Tree.
 *
 * Rules of Red-Black Tree (RBT).
 * 1. 	Each node of RBT has its color (black or red).
 * 2. 	The root must be black.
 * 3. 	The leafs are black (NIL).
 * 4. 	If a certain node is red, its two children must be black.
 *	That is, There cannot link two red nodes directly.
 * 5.	Starting from any node, the path between the start and any descendant
 *	leaf must have the same number of black nodes.
 * */

struct cmap_node {
	bool black;
	void *key;
	void *val;
	struct cmap_node *parent, *left, *right;
};

#define NIL &nil_node

static struct cmap_node nil_node = {.black = true,
				    .key = NULL,
				    .val = NULL,
				    .parent = NIL,
				    .left = NIL,
				    .right = NIL};

static int cmap_postorder(struct cmap_node *node) {
	int leftpath = 0, rightpath = 0;
	if (node != NIL) {
		if (node->black == false && (node->left->black == false ||
					     node->right->black == false)) {
			fprintf(stderr,
				"A red node has a least one red child node\n");
			exit(0);
		}
		leftpath = cmap_postorder(node->left);
		rightpath = cmap_postorder(node->right);
		if (leftpath != rightpath) {
			fprintf(stderr, "The length of the left path is "
					"inequal to the right path.\n");
			exit(0);
		}
	}
	return leftpath + node->black;
}

static void cmap_validate(cmap_t *map) {
	if (map->root->black == false) {
		fprintf(stderr, "The root's color of the cmap is not black\n");
		exit(0);
	}
	cmap_postorder(map->root);
	if (nil_node.black == false || nil_node.parent != NIL ||
	    nil_node.left != NIL || nil_node.right != NIL) {
		fprintf(stderr, "The properties of nil_node has been broken\n");
		exit(0);
	}
}

static void cmap_node_free(struct cmap_node *node) {
	free(node->key);
	free(node->val);
	free(node);
}

void *cmap_root_init(void) {
	return NIL;
}

cmap_t cmap_init(int (*cmp)(const void *, const void *), size_t (*key_size_get)(const void *), size_t (*val_size_get)(const void *)) {
	cmap_t map = {
		.root = NIL, .cmp = cmp, .key_size_get = key_size_get, .val_size_get = val_size_get,
		.search = cmap_search, .insert = cmap_insert, .erase = cmap_erase, .destroy = cmap_destroy
	};
	return map;
}

void *cmap_search(cmap_t *map, const void *key) {
	struct cmap_node **cursor = &map->root;
	while (*cursor != NIL) {
		int cmp = map->cmp((*cursor)->key, key);
		if (cmp == 0)
			return (*cursor)->val;
		else if (cmp < 0)
			cursor = &(*cursor)->right;
		else
			cursor = &(*cursor)->left;
	}
	return NULL;
}

static void cmap_left_rotation(cmap_t *map, struct cmap_node *node) {
	struct cmap_node *parent = node->parent;
	struct cmap_node *right = node->right;
	struct cmap_node **parent_child = NULL;
	if (node == parent->left)
		parent_child = &parent->left;
	else
		parent_child = &parent->right;

	if (parent != NIL)
		*parent_child = right;
	else
		map->root = right;
	right->parent = parent;

	node->right = right->left;

	if (right->left != NIL)
		right->left->parent = node;

	node->parent = right;
	right->left = node;
}

static void cmap_right_rotation(cmap_t *map, struct cmap_node *node) {
	struct cmap_node *parent = node->parent;
	struct cmap_node *left = node->left;
	struct cmap_node **parent_child = NULL;
	if (node == parent->left)
		parent_child = &parent->left;
	else
		parent_child = &parent->right;

	if (parent != NIL)
		*parent_child = left;
	else
		map->root = left;
	left->parent = parent;

	node->left = left->right;
	if (left->right != NIL)
		left->right->parent = node;

	node->parent = left;
	left->right = node;
}

static void cmap_insert_fixup(cmap_t *map, struct cmap_node *node) {
	while (node->parent->black == false) {
		struct cmap_node *parent = node->parent;
		struct cmap_node *grandparent = parent->parent;
		struct cmap_node *uncle = NIL;

		if (parent == grandparent->left) {
			uncle = grandparent->right;
		}
		else {
			uncle = grandparent->left;
		}

		if (uncle->black == false) {
			parent->black = true;
			uncle->black = true;
			grandparent->black = false;
			node = grandparent;
		}
		else if (parent == grandparent->left) {
			if (node == parent->right) {
				node = parent;
				cmap_left_rotation(map, node);
				parent = node->parent;
				grandparent = parent->parent;
			}
			parent->black = true;
			grandparent->black = false;
			cmap_right_rotation(map, grandparent);
		}

		else {
			if (node == parent->left) {
				node = parent;
				cmap_right_rotation(map, node);
				parent = node->parent;
				grandparent = parent->parent;
			}
			parent->black = true;
			grandparent->black = false;
			cmap_left_rotation(map, grandparent);
		}
	}
	map->root->black = true;
}

void cmap_insert(cmap_t *map, const void *key, const void *val) {
	struct cmap_node *prev_node = NIL;
	struct cmap_node **cursor = &map->root;
	while (*cursor != NIL) {
		prev_node = *cursor;
		int cmp = map->cmp((*cursor)->key, key);
		if (cmp == 0) {
			size_t new_size = map->val_size_get(val);
			(*cursor)->val = realloc((*cursor)->val, new_size);
			memcpy((*cursor)->val, val, new_size);
			goto INSERT_END;
		}
		else if (cmp < 0)
			cursor = &(*cursor)->right;
		else
			cursor = &(*cursor)->left;
	}

	// Allocating a new node.
	struct cmap_node *new_node = calloc(1, sizeof(struct cmap_node));
	size_t key_size = map->key_size_get(key);
	size_t val_size = map->val_size_get(val);
	new_node->key = calloc(1, key_size);
	memcpy(new_node->key, key, key_size);
	new_node->val = calloc(1, val_size);
	memcpy(new_node->val, val, val_size);
	new_node->left = new_node->right = new_node->parent = NIL;

	*cursor = new_node;
	new_node->parent = prev_node;
	cmap_insert_fixup(map, new_node);
INSERT_END:
#if DEBUG == 1
	cmap_validate(map);
#endif
	return;
}

static void cmap_erase_fixup(cmap_t *map, struct cmap_node *node) {
	
	while (node != map->root && node->black) {
		struct cmap_node *parent = node->parent;
		bool node_is_left = (node == parent->left);
		struct cmap_node *sibling =
			node_is_left ? parent->right : parent->left;
		if (sibling->black == false) {
			sibling->black = true;
			node->parent->black = false;
			if (node_is_left) {
				cmap_left_rotation(map, parent);
				sibling = parent->right;
			}
			else {
				cmap_right_rotation(map, parent);
				sibling = parent->left;
			}
		}
		if (sibling->left->black && sibling->right->black) {
			sibling->black = false;
			node = parent;
		}
		else if (node_is_left) {
			if (sibling->right->black) {
				sibling->black = false;
				sibling->left->black = true;
				cmap_right_rotation(map, sibling);
				sibling = parent->right;
			}
			sibling->black = parent->black;
			parent->black = true;
			sibling->right->black = true;
			cmap_left_rotation(map, parent);
			node = map->root;
		}
		else {
			if (sibling->left->black) {
				sibling->black = false;
				sibling->right->black = true;
				cmap_left_rotation(map, sibling);
				sibling = parent->left;
			}
			sibling->black = parent->black;
			parent->black = true;
			sibling->left->black = true;
			cmap_right_rotation(map, parent);
			node = map->root;
		}
	}
	node->black = true;
}

static struct cmap_node *cmap_node_successor(cmap_t *map,
					     struct cmap_node *node) {
	struct cmap_node *successor = node->right;
	while (successor->left != NIL)
		successor = successor->left;
	return successor;
}

// TODO: Now this function can cause an infinte loop.
bool cmap_erase(cmap_t *map, const void *key) {
	struct cmap_node **cursor = &map->root;
	while (*cursor != NIL) {
		int cmp = map->cmp((*cursor)->key, key);
		if (cmp == 0) {
			bool erase_black = false;
			struct cmap_node *successor =
				cmap_node_successor(map, (*cursor));
			struct cmap_node *erase_node = NIL;
			if (successor != NIL) {
				void *tmp = (*cursor)->key;
				(*cursor)->key = successor->key;
				successor->key = tmp;
				tmp = (*cursor)->val;
				(*cursor)->val = successor->val;
				successor->val = tmp;
				cursor = successor->parent->left == successor? 
					 &successor->parent->left : &successor->parent->right;
			}
			erase_node = (*cursor);
			erase_black = erase_node->black;
			(*cursor) = erase_node->left ? erase_node->left : erase_node->right;
			(*cursor)->parent = erase_node->parent;
			cmap_node_free(erase_node);
			if (erase_black) {
				cmap_erase_fixup(map, *cursor);
			}
			nil_node.black = true;
			nil_node.parent = NIL;
#if DEBUG == 1
			cmap_validate(map);
#endif
			return true;
		}
		else if (cmp < 0)
			cursor = &(*cursor)->right;
		else
			cursor = &(*cursor)->left;
	}
	return false;
}

static void cmap_node_destroy(struct cmap_node *node) {
	if (node != NIL) {
		cmap_node_destroy(node->left);
		cmap_node_destroy(node->right);
		cmap_node_free(node);
	}
}

void cmap_destroy(cmap_t *map) {
	cmap_node_destroy(map->root);
	map->root = NIL;
}

void *cmap_alloc(int (*cmp)(const void *, const void *),
		 size_t (*key_size_get)(const void *),
		 size_t (*val_size_get)(const void *)) {

	cmap_t map = CMAP_INIT(cmp, key_size_get, val_size_get);
	cmap_t *alloc_map = malloc(sizeof(cmap_t));
	memcpy(alloc_map, &map, sizeof(cmap_t));
	return alloc_map;
}

