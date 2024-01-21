#include "sorted_kv_array.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void SortedKVArrayInit(SortedKVArray* a, int key_size, int value_size,
		       CompareFn key_cmp_fn, FreeFn key_free_fn,
		       FreeFn value_free_fn) {
            a->key_cmp_fn = key_cmp_fn;
            a->key_free_fn = key_free_fn;
            a->key_size = key_size;
            a->value_size = value_size;
            a->value_free_fn = value_free_fn;
            a->log_len = 0;
            a->alloc_len = 4;
            a->base = malloc(a->alloc_len*(a->key_size + a->value_size)); 
}

void SortedKVArrayDestroy(SortedKVArray* a) {
  for (int i = 0; i < a->log_len; i++)
  {
    void* key = (char*)a->base + i*(a->key_size + a->value_size);
    void* value = (char*)a->base + i*(a->key_size + a->value_size) + a->key_size;
    if(a->key_free_fn != NULL){
      a->key_free_fn(key);
    }
    if(a->value_free_fn != NULL){
      a->value_free_fn(value);
    }
  }
  free(a->base);
}

void SortedKVArrayAdd(SortedKVArray* a, void* key, void* value) {
  for (int i = 0; i < a->log_len; i++){
    void* curr_key = (char*)a->base + i*(a->key_size + a->value_size);
    if(a->key_cmp_fn(curr_key, key) == 0){
      if(a->value_free_fn != NULL) a->value_free_fn((char*)curr_key + a->key_size);
        memcpy((char*)curr_key + a->key_size, value, a->value_size);
        if(a->key_free_fn != NULL) a->key_free_fn((char*)a->base+ i*(a->key_size+a->value_size));
        memcpy((char*)curr_key, key, a->key_size);
        return;
    } else if (a->key_cmp_fn(curr_key, key) > 0){
      if(a->log_len == a->alloc_len){
        a->alloc_len *= 2;
        a->base = realloc(a->base, a->alloc_len*(a->key_size + a->value_size));
      }
      memmove((char*)a->base + (i+1)*(a->key_size+a->value_size), (char*)a->base+ i*(a->key_size+a->value_size), (a->log_len - i) * (a->key_size + a->value_size));
      memcpy((char*)a->base+ i*(a->key_size+a->value_size), key, a->key_size);
      memcpy((char*)a->base+ i*(a->key_size+a->value_size) + a->key_size, value, a->value_size);
      a->log_len++;
      return;
    }
  }
  if(a->log_len == a->alloc_len){
    a->alloc_len *= 2;
    a->base = realloc(a->base, a->alloc_len*(a->key_size + a->value_size));
  }
  void* curr_key = (char*)a->base + a->log_len*(a->key_size + a->value_size);
  void* curr_value = (char*) curr_key + a->key_size;
  memcpy((char*)curr_key, key, a->key_size);
  memcpy((char*)curr_value, value, a->value_size);
  a->log_len++;
}

bool SortedKVArrayRemove(SortedKVArray* a, void* key) {
  for (int i = 0; i < a->log_len; i++){
    void* curr_key = (char*)a->base + i*(a->key_size + a->value_size);
    if(a->key_cmp_fn(curr_key, key) == 0){
      void* curr_value = (char*) curr_key + a->key_size;
      if(a->value_free_fn != NULL){
        a->value_free_fn(curr_value);
      }
      if(a->key_free_fn != NULL){
        a->key_free_fn(curr_key);
      }

      memmove(curr_key, (char*)curr_key + (a->key_size+a->value_size), (a->log_len - 1 - i) * (a->key_size+a->value_size));
      a->log_len--;
      return true;
    }
  }
  return false;
}

int SortedKVArraySize(SortedKVArray* a) {
  return a->log_len;
}

void* SortedKVArrayGetKey(SortedKVArray* a, int index) {
  return (char*) a->base + index* (a->key_size + a->value_size);
}

void* SortedKVArrayGetValue(SortedKVArray* a, void* key) {
  for (int i = 0; i < a->log_len; i++){
    void* curr_key = (char*)a->base + i*(a->key_size + a->value_size);
    if(a->key_cmp_fn(curr_key, key) == 0){
      void* curr_value = (char*) curr_key + a->key_size;
      return curr_value;
    }
  }
  return NULL;
}
