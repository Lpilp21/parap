#ifndef PARADIGMS_PROBLEMS_SORTED_KV_ARRAY_SORTED_KV_ARRAY_H_
#define PARADIGMS_PROBLEMS_SORTED_KV_ARRAY_SORTED_KV_ARRAY_H_

#include <stdbool.h>

// Returns 0 if two passed elements are equal.
// Returns negative number if first element is less than second one.
// Returns positive number otherwise.
typedef int(*CompareFn)(void*, void*);

// Must free up memory at given pointer.
typedef void(*FreeFn)(void*);

typedef struct {
  void* base;
  int key_size;
  int value_size;
  int log_len;
  int alloc_len;
	CompareFn key_cmp_fn;
  FreeFn key_free_fn;
	FreeFn value_free_fn;
} SortedKVArray;

// Initializes given array.
void SortedKVArrayInit(SortedKVArray* a, int key_size, int value_size,
		       CompareFn key_cmp_fn, FreeFn key_free_fn,
		       FreeFn value_free_fn);

// Frees up all dynamically allocated memory used by given array.
void SortedKVArrayDestroy(SortedKVArray* a);

// Adds new key/value pair to the array.
// Takes ownership of both key and value pointers.
void SortedKVArrayAdd(SortedKVArray* a, void* key, void* value);

// Removes key/value pair associated with given key.
// Returns true if key was found, false otherwise.
// Does not take ownerhip of give key pointer.
bool SortedKVArrayRemove(SortedKVArray* a, void* key);

// Returns size of the given array.
int SortedKVArraySize(SortedKVArray* a);

// Returns address of the key stored at given index.
void* SortedKVArrayGetKey(SortedKVArray* a, int index);

// Gets address of the value associated with the given key.
// Returns NULL if no such key was found.
// Does not take ownership of given key pointer.
// Does not give ownership of returned pointer to the caller.
void* SortedKVArrayGetValue(SortedKVArray* a, void* key);

#endif // PARADIGMS_PROBLEMS_SORTED_KV_ARRAY_SORTED_KV_ARRAY_H_
