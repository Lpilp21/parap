#ifndef PARADIGMS_PROBLEMS_MAPSET_SORTED_SET_H_
#define PARADIGMS_PROBLEMS_MAPSET_SORTED_SET_H_

#include "vector.h"

typedef int(*CmpFn)(void* a, void* b);

typedef struct {
  // არ გაქვთ ამ სტრუქტურის შეცვლის უფლება.
  Vector v;
  CmpFn cmp_fn;
  FreeFn free_fn;
} SortedSet;

void SortedSetInit(SortedSet* s, int elem_size, CmpFn cmp_fn, FreeFn free_fn);
void SortedSetDestroy(SortedSet* s);
int SortedSetInsert(SortedSet* s, void* elem);
void* SortedSetGet(SortedSet* s, int index);
int SortedSetSize(SortedSet* s);

#endif  // PARADIGMS_PROBLEMS_MAPSET_SORTED_SET_H_
