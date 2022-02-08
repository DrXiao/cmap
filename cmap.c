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

/**
 * struct cmap_node - the information of a node used by cmap.
 * @black: 	color of a node. (either red or black)
 * @key:	key of a node.
 * @val:	value of a node.
 * @parent:	pointer to parent of the node.
 * @left:	pointer to left subtree of the node.
 * @right	pointer to right subtree of the node.
 *
 * A simple definition of a cmap node to imitate <map> container in C++.
 */
struct cmap_node {
	bool black;
	cmap_data_t key;
	cmap_data_t val;
	cmap_node_t *parent, *left, *right;

	int (*const cmp)(cmap_node_t *, const void *);
	void (*const insert_key)(cmap_node_t *, const void *);
	void (*const insert_val)(cmap_node_t *, const void *);
	void (*const destroy)(cmap_node_t *);
	void (*const dealloc)(void *);
};


/* 
 * Functions for struct cmap_node
 * 
 * cmap_node_init(): 		Constructor(Initialization) for a cmap node.
 * cmap_node_cmp():		Comparsion between the key of a cmap node and another key.
 * cmap_node_insert_key():	Insert a new key into the key field of a given cmap node.
 * cmap_node_insert_val():	Insert a new value into the val field of a given cmap node.
 * cmap_node_alloc():		Allocation for a cmap node.
 * cmap_destroy():		Destructor for a given cmap node.
 *
 * All details about the above functions are mentioned at their implementation places.
 */
static cmap_node_t cmap_node_init(cmap_t *map, const void *key, const void *val);
static int cmap_node_cmp(cmap_node_t *node, const void *key); 
static void cmap_node_insert_key(cmap_node_t *node, const void *key);
static void cmap_node_insert_val(cmap_node_t *node, const void *val); 
static void *cmap_node_alloc(cmap_t *map, const void *key, const void *val);
static void cmap_node_destroy(cmap_node_t *node);

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

/**
 * cmap_node_init - constructor of cmap node.
 * @map:	an object of cmap.
 * @key:	the given key inserted into the node.
 * @val:	the given value inseted into the node.
 * 
 * This is the constructor for the struct (or class, in the OOP opinion) cmap node.
 * Except for key and val, the resaon why needs to pass a cmap object into the function is
 * assigning the methods' implementations, whcih are provided by a cmap object, for key and 
 * value in a cmap node object.
 *
 * So, a cmap node can manipulate key and value like objects to use the given methods such as 
 * cmp() or copy() to do the relevant and needed operations.
 *
 * A cmap node is also used as an object so there has some function pointers, whcih are assigned
 * the specific functions, in a cmap node and.
 */
static cmap_node_t cmap_node_init(cmap_t *map, const void *key, const void *val) {
	cmap_node_t node = {.black = false,
			    .key = map->key_interface,
			    .val = map->val_interface,
			    .parent = NIL,
			    .left = NIL,
			    .right = NIL,
			    .cmp = cmap_node_cmp,
			    .insert_key = cmap_node_insert_key,
			    .insert_val = cmap_node_insert_val,
			    .destroy = cmap_node_destroy,
			    .dealloc = free};
	node.insert_key(&node, key);
	node.insert_val(&node, val);
	return node;
}

/**
 * cmap_node_cmp - doing comparsion between the key of a node and another key.
 * 
 * Because of involving comparsion of keys when searching, inserting and deleting,
 * this function, also being a method of a cmap node, is calling cmp() method of the 
 * key in the cmap node to compare another key, then returning its result..
 */
static int cmap_node_cmp(cmap_node_t *node, const void *key) {
	return node->key.cmp(node->key.data, key);
}

/**
 * cmap_node_insert_key - inserting a given key into a cmap node.
 * @node:	an object of a cmap node.
 * @key:	a given key.
 *
 * This is a method of a cmap node.
 * The bahavior is that deallocates the original data in the cmap node 
 * (Actually, it always has no data because every new key doesn't exist in
 * the cmap object before inserting.)
 * then allocating an appropriate size for the new data.
 * Finally, copying the data from a given key to the key of the cmap node by
 * calling copy() method of the key.
 */
static void cmap_node_insert_key(cmap_node_t *node, const void *key) {
	size_t new_key_size = node->key.data_size_get(key);
	if (node->key.data != NULL) {
		if (node->key.destroy)
			node->key.destroy(node->key.data);
		node->key.dealloc(node->key.data);
	}
	node->key.data = calloc(1, new_key_size);
	node->key.copy(node->key.data, key, new_key_size);
}

/**
 * cmap_node_insert_val - inserting a given value into a cmap node.
 * @node:	an object of a cmap node.
 * @val:	a given value.
 *
 * This is a method of a cmap node.
 * The bahavior is that deallocates the original data in the cmap node 
 * 
 * Because insertig a new value are happened when inserting a new key or updating 
 * a new value for an existed key.
 * For the later situation, it must clear the previously old value before inserting 
 * the new value. Therefore, this function, which acts as cmap_node_insert_key() but has 
 * the important difference mentioned before, deallocates the original data then allocate
 * and copy the new data of a given value.
 */
static void cmap_node_insert_val(cmap_node_t *node, const void *val) {
	size_t new_val_size = node->val.data_size_get(val);
	if (node->val.data != NULL) {
		if (node->val.destroy)
			node->val.destroy(node->val.data);
		node->val.dealloc(node->val.data);
	}
	node->val.data = calloc(1, new_val_size);
	node->val.copy(node->val.data, val, new_val_size);
}

/**
 * cmap_node_alloc - allocating an cmap node object.
 * @map:	a cmap object.
 * @key:	a given key	
 * @val:	a given value
 * 
 * Needed to store cmap nodes dynamically, A cmap object creates a cmap node object by
 * calling this function to allocate a cmap node.
 * It calls calloc() and cmap_node_init() to allocate and initialize a cmap node object.
 */
static void *cmap_node_alloc(cmap_t *map, const void *key, const void *val) {
	cmap_node_t node = cmap_node_init(map, key, val);
	cmap_node_t *alloc_node = calloc(1, sizeof(cmap_node_t));
	memcpy(alloc_node, &node, sizeof(cmap_node_t));
	return alloc_node;
}

/**
 * cmap_node_destroy - destructor for cmap node.
 * @node:	an object of cmap node.
 * 
 * Created by allocation, the way to destroy a cmap node is calls
 * a recursive function to deallocate its left and right subtrees then
 * destroy itself.
 *
 * The process is a postorder traversal and destroies and deallocates key and 
 * value objects, which may be given destroy() implementations by user, then
 * the node calls dealloc() method to free itself.
 */
static void cmap_node_destroy(cmap_node_t *node) {
	if (node != NIL) {
		node->destroy(node->left);
		node->destroy(node->right);
		if (node->key.destroy)
			node->key.destroy(node->key.data);
		if (node->val.destroy)
			node->val.destroy(node->val.data);
		node->key.dealloc(node->key.data);
		node->val.dealloc(node->val.data);
		node->dealloc(node);
	}
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
static void *cmap_search(cmap_t *map, const void *key);
static void cmap_insert(cmap_t *map, const void *key, const void *val); 
static void cmap_insert_fixup(cmap_t *map, cmap_node_t *node);
static bool cmap_erase(cmap_t *map, const void *key); 
static void cmap_erase_fixup(cmap_t *map, cmap_node_t *node); 
static cmap_node_t *cmap_node_successor(cmap_t *map, cmap_node_t *node);
static void cmap_destroy(cmap_t *);

cmap_t cmap_init(cmap_data_t *key_interface, cmap_data_t *val_interface) {
	cmap_t map = {.root = NIL,
		      .key_interface = *key_interface,
		      .val_interface = *val_interface,
		      .search = cmap_search,
		      .insert = cmap_insert,
		      .erase = cmap_erase,
		      .destroy = cmap_destroy,
		      .dealloc = free};
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
			erase_node->left = erase_node->right = NIL;
			erase_node->destroy(erase_node);
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
