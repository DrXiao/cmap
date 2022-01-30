#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "cmap.h"
#define DEBUG 1
#if DEBUG == 1
#include <stdio.h>
#endif

/**
 * C map -	An associative array implemented by C language.
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
 */

static cmap_node_t cmap_node_init(cmap_t *map, const void *key, const void *val);
static int cmap_node_cmp(const void *node_ptr, const void *key); 
static void cmap_node_def_insert_key(cmap_node_t *node, const void *key);
static void cmap_node_def_insert_val(cmap_node_t *node, const void *val); 
static void *cmap_node_alloc(cmap_t *map, const void *key, const void *val);
static void cmap_node_destroy(cmap_node_t *node);
static void cmap_node_free(cmap_node_t *node);

/**
 * For the theory of red-black tree, it has a special node called NIL
 * (or NEEL) to represent the leaf, and it has no data and is black forever.
 *
 * So, here defines it for the entire implementations.
 */
#define NIL &nil_node
static cmap_node_t nil_node = {.black = true,
			       .key = {NULL},
			       .val = {NULL},
			       .parent = NIL,
			       .left = NIL,
			       .right = NIL,
			       .destroy = cmap_node_destroy};

static cmap_node_t cmap_node_init(cmap_t *map, const void *key, const void *val) {
	cmap_node_t node = {.black = false,
			    .key = map->key_interface,
			    .val = map->val_interface,
			    .parent = NIL,
			    .left = NIL,
			    .right = NIL,
			    .cmp = cmap_node_cmp,
			    .insert_key = cmap_node_def_insert_key,
			    .insert_val = cmap_node_def_insert_val,
			    .destroy = cmap_node_destroy};
	node.insert_key(&node, key);
	node.insert_val(&node, val);
	return node;
}

static int cmap_node_cmp(const void *node_ptr, const void *key) {
	cmap_node_t *node = (cmap_node_t *)node_ptr;
	return node->key.cmp(node->key.data, key);
}

static void cmap_node_def_insert_key(cmap_node_t *node, const void *key) {
	size_t new_key_size = node->key.data_size_get(key);
	node->key.data = realloc(node->key.data, new_key_size);
	node->key.copy(node->key.data, key, new_key_size);
}

static void cmap_node_def_insert_val(cmap_node_t *node, const void *val) {
	size_t new_val_size = node->val.data_size_get(val);
	node->val.data = realloc(node->val.data, new_val_size);
	node->val.copy(node->val.data, val, new_val_size);
}

static void *cmap_node_alloc(cmap_t *map, const void *key, const void *val) {
	cmap_node_t node = cmap_node_init(map, key, val);
	cmap_node_t *alloc_node = calloc(1, sizeof(cmap_node_t));
	memcpy(alloc_node, &node, sizeof(cmap_node_t));
	return alloc_node;
}

static void cmap_node_destroy(cmap_node_t *node) {
	if (node != NIL) {
		node->destroy(node->left);
		node->destroy(node->right);
		cmap_node_free(node);
	}
}

static void cmap_node_free(cmap_node_t *node) {
	node->key.destroy(node->key.data);
	node->val.destroy(node->val.data);
	free(node);
}

#if DEBUG == 1
/**
 * cmap_postorder() is used by cmap_validate() to help it to
 * verify the correctness of a cmap object.
 *
 * It checks the pointers of a cmap_node, colors between parent and children
 * and the numbers of black nodes between the left path and right path.
 *
 * If it is correct at this subtree, returning the number of the black nodes.
 */
static int cmap_postorder(cmap_node_t *node) {
	int leftpath = 0, rightpath = 0;
	if (node != NIL) {
		if ((node->left != NIL && node->left->parent != node) ||
		    (node->right != NIL && node->right->parent != node)) {
			fprintf(stderr, "The child's parent is inequal to the "
					"actual parent\n");
			exit(0);
		}
		if (node->black == false && (node->left->black == false ||
					     node->right->black == false)) {
			fprintf(stderr,
				"A red node has a least one red child node\n");
			exit(0);
		}
		leftpath = cmap_postorder(node->left);
		rightpath = cmap_postorder(node->right);
		if (leftpath != rightpath) {
			fprintf(stderr, "The path of the left is "
					"inequal to the right.\n");
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
#endif

cmap_t cmap_init(cmap_data_t *key_interface, cmap_data_t *val_interface); 
void *cmap_alloc(cmap_data_t *key_interface, cmap_data_t *val_interface); 
static void cmap_left_rotation(cmap_t *map, cmap_node_t *node);
static void cmap_right_rotation(cmap_t *map, cmap_node_t *node); 
void *cmap_search(cmap_t *map, const void *key);
void cmap_insert(cmap_t *map, const void *key, const void *val); 
static void cmap_insert_fixup(cmap_t *map, cmap_node_t *node);
bool cmap_erase(cmap_t *map, const void *key); 
static void cmap_erase_fixup(cmap_t *map, cmap_node_t *node); 
static cmap_node_t *cmap_node_successor(cmap_t *map, cmap_node_t *node);

cmap_t cmap_init(cmap_data_t *key_interface, cmap_data_t *val_interface) {
	cmap_t map = {.root = NIL,
		      .key_interface = *key_interface,
		      .val_interface = *val_interface,
		      .search = cmap_search,
		      .insert = cmap_insert,
		      .erase = cmap_erase,
		      .destroy = cmap_destroy,
		      .insert_key = cmap_node_def_insert_key,
		      .insert_val = cmap_node_def_insert_val};
	map.key_interface.data = map.val_interface.data = NULL;
	return map;
}

void *cmap_alloc(cmap_data_t *key_interface, cmap_data_t *val_interface) {
	cmap_t map = cmap_init(key_interface, val_interface);
	cmap_t *alloc_map = malloc(sizeof(cmap_t));
	memcpy(alloc_map, &map, sizeof(cmap_t));
	return alloc_map;
}

static void cmap_left_rotation(cmap_t *map, cmap_node_t *node) {
	cmap_node_t *parent = node->parent;
	cmap_node_t *right = node->right;
	cmap_node_t **parent_child = NULL;
	if (node == parent->left)
		parent_child = &parent->left;
	else
		parent_child = &parent->right;

	node->right = right->left;

	if (right->left != NIL)
		right->left->parent = node;

	right->parent = parent;

	if (parent != NIL)
		*parent_child = right;
	else
		map->root = right;

	right->left = node;
	node->parent = right;
}

static void cmap_right_rotation(cmap_t *map, cmap_node_t *node) {
	cmap_node_t *parent = node->parent;
	cmap_node_t *left = node->left;
	cmap_node_t **parent_child = NULL;
	if (node == parent->left)
		parent_child = &parent->left;
	else
		parent_child = &parent->right;

	node->left = left->right;

	if (left->right != NIL)
		left->right->parent = node;

	left->parent = parent;

	if (parent != NIL)
		*parent_child = left;
	else
		map->root = left;
	left->right = node;
	node->parent = left;
}

void *cmap_search(cmap_t *map, const void *key) {
	cmap_node_t **cursor = &map->root;
	while (*cursor != NIL) {
		int cmp = (*cursor)->cmp((*cursor), key);
		if (cmp == 0)
			return (*cursor)->val.data;
		else if (cmp < 0)
			cursor = &(*cursor)->right;
		else
			cursor = &(*cursor)->left;
	}
	return NULL;
}

void cmap_insert(cmap_t *map, const void *key, const void *val) {
	cmap_node_t *prev_node = NIL;
	cmap_node_t **cursor = &map->root;
	while (*cursor != NIL) {
		prev_node = *cursor;
		int cmp = (*cursor)->cmp((*cursor), key);
		if (cmp == 0) {
			(*cursor)->insert_val((*cursor), val);
			goto INSERT_END;
		}
		else if (cmp < 0)
			cursor = &(*cursor)->right;
		else
			cursor = &(*cursor)->left;
	}

	// Allocating a new node.
	cmap_node_t *new_node = cmap_node_alloc(map, key, val);
	*cursor = new_node;
	new_node->parent = prev_node;
	cmap_insert_fixup(map, new_node);
INSERT_END:
#if DEBUG == 1
	cmap_validate(map);
#endif
	return;
}

static void cmap_insert_fixup(cmap_t *map, cmap_node_t *node) {
	while (node->parent->black == false) {
		cmap_node_t *parent = node->parent;
		cmap_node_t *grandparent = parent->parent;
		cmap_node_t *uncle = NIL;

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

bool cmap_erase(cmap_t *map, const void *key) {
	cmap_node_t **cursor = &map->root;
	while (*cursor != NIL) {
		int cmp = (*cursor)->cmp((*cursor), key);
		if (cmp == 0) {
			bool erase_black = false;
			cmap_node_t *successor =
				cmap_node_successor(map, (*cursor));
			cmap_node_t *erase_node = NIL;
			if (successor != NIL) {
				void *tmp = (*cursor)->key.data;
				(*cursor)->key.data = successor->key.data;
				successor->key.data = tmp;
				tmp = (*cursor)->val.data;
				(*cursor)->val.data = successor->val.data;
				successor->val.data = tmp;
				cursor = successor->parent->left == successor
						 ? &successor->parent->left
						 : &successor->parent->right;
			}
			erase_node = (*cursor);
			erase_black = erase_node->black;
			(*cursor) = erase_node->left != NIL ? erase_node->left
							    : erase_node->right;
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

static void cmap_erase_fixup(cmap_t *map, cmap_node_t *node) {

	while (node != map->root && node->black) {
		cmap_node_t *parent = node->parent;
		bool node_is_left = (node == parent->left);
		cmap_node_t *sibling =
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

static cmap_node_t *cmap_node_successor(cmap_t *map, cmap_node_t *node) {
	cmap_node_t *successor = node->right;
	while (successor->left != NIL)
		successor = successor->left;
	return successor;
}

void cmap_destroy(cmap_t *map) {
	map->root->destroy(map->root);
	map->root = NIL;
}
