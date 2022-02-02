# C Map
## An associative array implemented by C laugnage.
* This is an implementation of an associative array. it is similar as C++ ```<map>``` but it is implemented by C; thus, this repo calls it **cmap** (C's ```<map>```).

## Implementation
* The goal of this repo is providing a binary search tree for the programmers using C language to work on thier projects. But, here is only a simple implementation of **Red-Black Tree**, which is known as a **self-balancing binary search tree**, and hopes it can be a helpful data structure for the projects written by C language.
* cmap is an implementation with OOP style; when using cmap, you must manipulate it as an 'object' in any OOP language.

## Usage
* Before using cmap, you have to define the three functions with the specific types by yourself.
```c
// A comparator for keys of Red-Black Tree.
int cmp(const void *key1, const void *key2) {
	/* return negative value, zero, positive value. */;
	/* You may understand this quickly if you had used qsort in your C projects. */
}

// A function for getting the memory size of key.
size_t key_size_get(const void *key) {
	/* returning the size of key. */
}

// A function for getting the memory size of value.
size_t val_size_get(const void *val) {
	/* returning the size of value */
}
```

### Example for using cmap.

[test/main.c](test/main.c)

## Methods of cmap.
```c
void *(*const search)(cmap_t *, const void *);
void (*const insert)(cmap_t *, const void *, const void *);
bool (*const erase)(cmap_t *, const void *);
void (*const destroy)(cmap_t *);
void (*const dealloc)(void *);
```
### TODO
* All of functions are finished but can be improved more efficient.
* Red-Black Tree guideline/documentation.
* The complete documentation of the cmap.
* Performance improvement of the cmap (More efficient implementations about all functions.)
* Other operations about Red-Black Tree.

## Build
* The following are the test files to validate the correctness of the cmap.
* If you want to test them, please modify the value of the macro ```DEBUG``` to be 1 in [cmap.c](cmap.c)
```c
// Release mode
#define DEBUG 0

// Debug mode
#define DEBUG 1
```
### Simple test files
1. A simple test file: [test/main.c](test/main.c)
	* This shows how to construct and use a cmap, and some of details has been descripted above.
	* In this case, The types of key and value of the cmap are C string and integer, respectively.
```
$ make main.elf
```
2. Test the insertion of the cmap: [test/test1.c](test/test1.c)
	* This program also test a cmap, whose types is identified as the cmap in [test/main.c](test/main.c)
	* There has twelve nodes constructed by the program and input some test data from [test/test.in](test/test.in) to search the cmap and get the corresponding value.
```
$ make test1.elf
```
3. Test the erasion of the cmap: [test/test2.c](test/test2.c)
	* The types of the cmap is also identified as the above tests.
	* It also has several ndoes and uses erase() method to test its the behavior.
```
$ make test2.elf
```
4. More complex insertions: [test/test3.c](test/test3.c)
	* It inserts the data from [test/test.in](test/test.in) to test more complex insertions.
```
make test3.elf
```
5. More complex erasions: [test/test4.c](test/test4.c)
	* It also inserts all data from [test/test.in](test/test.in), then removing all of them from the cmap object.
```
$ make test4.elf
```
* All of the above make targets will execute their binary executables automatically.
### Complex test files
1. A test file: [adv/adv1.c](adv/adv1.c)
	* It is a relatively simple test file to test the correctness of memory allocation maintained by cmap.
	* Its value type is C string and this file inserts a very long string for an existed key.
		* It tests whether there has any memory error when restoring a string from short to long.
```
$ make adv1.elf
```
2. A complex data type - linked list: [adv/adv2.c](adv/adv2.c)
	* In this case, the key and value types of cmap are integer and **linked lsit**, respectively.
	* When you create your linked list data type, there has some details when defining the relative functions.
		* Ex: 
			* How to destroy entire linked list when it is not used? 
			* How to copy a linked list from another linked list?
		* Because of the complexity of linked list, you should use ```CREATE_INTERFACE4``` to create a
		  complete interface to define the behaviors of all methods of ```struct cmap_data``` so that
		  cmap can manipulate the linked list correctly.
```
$ make adv2.elf
```
### Create the library (shared object)
* You can use the cmap by compiling its shared object to your binary exectuable, so here provides a target in makefile to create the shared object in a folder called ```bin```.
* This target is set by deafult and It also puts the cmap.h in the folder.
```
$ make
or
$ make build
```

