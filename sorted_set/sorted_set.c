#include "sorted_set.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

void SortedSetInit(SortedSet* s, int elem_size, CmpFn cmp_fn, FreeFn free_fn) {
  Vector v;
  s->v = v;
  s->cmp_fn = cmp_fn;
  s->free_fn = free_fn;
  VectorInit(&(s->v), elem_size, free_fn);
}
typedef struct{
  int index;
  void* elem;
}SetAux;

void MyMapFn(int index, void* elem, void* aux){
  SetAux* castedAux = (SetAux*) aux;
  if(index == castedAux-> index){
    castedAux->elem = elem;
    castedAux->index = index;
  }
}

void* Get(Vector* v,int index){
  SetAux aux;
  aux.index = index;
  aux.elem = NULL;
  VectorMap(v, MyMapFn, &aux);
  void* adr = aux.elem;
  return adr;
}
void SortedSetDestroy(SortedSet* s) {
  VectorDestroy(&(s->v));
}

int SortedSetInsert(SortedSet* s, void* elem) {
  int size = VectorSize(&(s->v));
  for(int i = 0; i < size; i++){
    void* got = Get(&(s->v), i);
    if(s->cmp_fn(elem, got) == 0){
      return -1;
    }
    if(s->cmp_fn(elem, got) < 0){
      VectorInsert(&(s->v), i, elem);
      return i;
    }
  }
  VectorInsert(&(s->v), size, elem);
  return size;
}

void* SortedSetGet(SortedSet* s, int index) {
  int el = *((int*)Get(&(s->v), index));
  return Get(&(s->v), index);
}

int SortedSetSize(SortedSet* s) {
  return VectorSize(&(s->v));
}
