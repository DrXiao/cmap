# C Map
## An associative array implemented by C laugnage.
* This is an implementation of an associative array written by C. it is similar as C++ ```<map>``` but it is implemented by C; thus, this repo calls it **cmap** (C++ <map>).

## Implementation
* The goal of this repo is providing a binary search tree for the programmers using C language to work on thier projects. But, here is only a simple implementation of **Red-Black Tree**, which is known as a **self-balancing binary search tree**, and hopes it can be a helpful data structure for the projects written by C language.
* cmap is an implementation with OOP style; when using cmap, you must manipulate it as an 'object' in any OOP language.

## Usage
* Before using cmap, you have to define the three functions with the specific types by yourself.
```c=
// A comparator for keys of Red-Black Tree.
int cmp(const void *key1, const void *key2) {
	/* return -1, 0, 1. */;
	/* You may understant this quickly if you had used qsort in your C projects. */
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

['test/main.c'](test/main.c)

## Methods of cmap.
```
void *(*const search)(cmap_t *, const void *);
void (*const insert)(cmap_t *, const void *, const void *);
bool (*const erase)(cmap_t *, const void *);
void (*destroy)(cmap_t *);
```
### TODO
* ```erase``` is incomplete, and others are finished but can be improved more efficient.
* Other operators about Red-Black Tree.

## Build
1. A simple test file: ['test/main.c'](test/main.c)
	* This shows how to construct and use a cmap, and some of details has been descripted above.
	* In this case, The types of key and value of cmap are C string and integer, respectively.
```
make build
./main
```

2. A test file to test Red-Black Tree: ['test/test1.c'](test/test1.c)
	* This program also test a cmap, whose types is identified as the cmap in ['test/main.c'](test/main.c)
	* There has twelve nodes constructed by the program and input some test data from ['test/test.in'](test/test.in) to search the cmap and get the corresponding value.
```
make test1
```

## Better usage
* (To be continued...)
