#ifndef __C_MAP__
#define __C_MAP__
#include <stdlib.h>
#include <stdbool.h>
#define CREATE_INTERFACE(cmp_func, size_get_func)                              \
	{.data = NULL,                                                         \
	 .cmp = cmp_func,                                                      \
	 .data_size_get = size_get_func,                                       \
	 .copy = memcpy,                                                       \
	 .destroy = NULL,                                                      \
	 .dealloc = free}

#define CREATE_INTERFACE4(cmp_func, size_get_func, copy_func, destroy_func)    \
	{.data = NULL,                                                         \
	 .cmp = cmp_func,                                                      \
	 .data_size_get = size_get_func,                                       \
	 .copy = copy_func,                                                    \
	 .destroy = destroy_func,                                              \
	 .dealloc = free}

typedef struct cmap cmap_t;
typedef struct cmap_node cmap_node_t;
typedef struct cmap_data cmap_data_t;

/**
 * struct cmap_data - the structure containing your data and necessary function pointers.
 * @data:		pointer to your data allocated by memory allocation.
 * @cmp:		function pointer returning the comparsion between two objects
 *			with the same type.
 * @data_size_get:	function pointer returning the memory size of the data.
 * @copy:		function pointer to copy the entire object to another.
 * @destroy:		function pointer to destroy the object. (That is, It is a destructor.)
 * @dealloc:		function pointer to dealloc the object if the object is allocated by 
 *			memory allocation. (pointed to free() function.)
 *
 * In order to comply the essence of OOP, data in cmap should be an object having its
 * data and methods, so the following structure has two usages.
 *
 * 1. Maintain the user's data by cmap: cmap will use the following structure to store
 *					the user's data and calls its methods to do the
 *					necessary operations.
 *
 * 2. Define the methods by user:	At least, user should define the cmp and data_size_get
 *					functions to tell cmap how to do comparsion between objects
 *					and what the size of the user's data is.
 *
 *					If you only define cmp and data_size_get functions, you can use
 *					CREATE_INTERFACE to create a object with function pointers to your
 *					functions but without no data.
 *
 *					For copy, destroy and free, their default behaviors are memset(), 
 *					no action (NULL pointer) and free().
 *					
 *					It is necessary to define copy and destroy functions if your data 
 *					is a complex data type which may need to do complicated memory management.
 *					If they are defined, using CREATE_INTERFACE4 to create an object with function pointers
 *					to the corresponding functions.
 */
struct cmap_data {
	void *data;
	int (*const cmp)(const void *, const void *);
	size_t (*const data_size_get)(const void *);
	void *(*const copy)(void *, const void *, size_t);
	void (*const destroy)(void *);
	void (*const dealloc)(void *);
};

/**
 * struct cmap - the structure of cmap.
 * @root:		pointer to the root of Red-Black Tree.
 * 			The definition of cmap_node_t has been defined and hidden in cmap.c file, so
 * 			users don't need to realize the data of cmap_node_t (Not important).
 * @key_interface:	An instance of cmap_data with valid function pointers for key in cmap.
 * @val_interface:	An instance of cmap_data with valid function pointers for value in cmap.
 * @search:		A function pointer to a built-in function to search a specific key, then  returning
 *			the pointer to the corresponding value (data field of val with cmap_data_t type in cmap_node_t.)
 * @insert:		A function pointer to a built-in function to insert or update a value for a certain key.
 * @erase:		A function pointer to a built-in function to erase a node with assigned key from cmap.
 * @destroy:		A function pointer to a built-in function to destroy the object of cmap.
 * @dealloc:		A function pointer to free() providing a method to free an allocated object of cmap for user.
 *
 * This is the main structure of cmap for user to manipulate a Red-Black Tree like <map> in C++.
 * Except for key_interface and val_interface, which should be defined by user, other function pointers such as search,
 * insert, and erase are the methods of cmap! User can manipulate an object of cmap to maintain their data in their projects.
 */
struct cmap {
	cmap_node_t *root;
	cmap_data_t key_interface;
	cmap_data_t val_interface;
	void *(*const search)(cmap_t *, const void *);
	void (*const insert)(cmap_t *, const void *, const void *);
	bool (*const erase)(cmap_t *, const void *);
	void (*const destroy)(cmap_t *);
	void (*const dealloc)(void *);
};


/**
 * cmap_init - A function returning an instance of cmap created via two instances of cmap_data_t.
 * @key_interface:	An instance of cmap_data with valid function pointers for key in cmap.
 * @val_interface:	An instance of cmap_data with valid function pointers for value in cmap.
 * 
 * The above parameters should be created by user first, then passing them when calling this function.
 */
cmap_t cmap_init(cmap_data_t *key_interface, cmap_data_t *val_interface);


/**
 * cmap_init - A function returning a pointer to an allocated instance of cmap.
 * @key_interface:	An instance of cmap_data with valid function pointers for key in cmap.
 * @val_interface:	An instance of cmap_data with valid function pointers for value in cmap.
 * 
 * The above parameters should be created by user first, then passing them when calling this function.
 * It will calls malloc() to allocate an object initialized by cmap_init().
 */
void *cmap_alloc(cmap_data_t *ker_interface, cmap_data_t *val_interface);

#endif
