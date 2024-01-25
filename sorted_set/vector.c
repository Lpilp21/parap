#include "vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NTH(v, i) (void*)((char*)(v)->base + (i) * (v)->elem_size)

void VectorInit(Vector* v, int elem_size, FreeFn free_fn) {
  v->log_len = 0;
  v->alloc_len = 4;
  v->base = malloc(v->alloc_len * elem_size);
  assert(v->base != NULL);
  v->elem_size = elem_size;
  v->free_fn = free_fn;
}

void VectorDestroy(Vector* v) {
  if (v->free_fn != NULL) {
    for (int i = 0; i < v->log_len; ++i) {
      v->free_fn(NTH(v, i));
    }
  }
  free(v->base);
}

int VectorSize(Vector* v) {
  return v->log_len;
}

void VectorInsert(Vector* v, int index, void* elem) {
  assert(0 <= index && index <= v->log_len);
  if (v->log_len == v->alloc_len) {
    v->alloc_len *= 2;
    v->base = realloc(v->base, v->alloc_len * v->elem_size);
    assert(v->base != NULL);
  }
  memmove(NTH(v, index + 1), NTH(v, index), (v->log_len - index) * v->elem_size);
  memcpy(NTH(v, index), elem, v->elem_size);
  v->log_len++;
}

void VectorMap(Vector* v, MapFn map_fn, void* aux) {
  for (int i = 0; i < v->log_len; ++i) {
    map_fn(i, NTH(v, i), aux);
  }
}
