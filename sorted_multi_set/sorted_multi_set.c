#include "sorted_multi_set.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void SortedMultiSetInit(SortedMultiSet* s, int elem_size, CmpFn cmp_fn, FreeFn free_fn) {
  // IMPLEMENT
  s->cmp_fn = cmp_fn;
  s->free_fn = free_fn;
  s->elem_size = elem_size;
  VectorInit(&s->v, elem_size, free_fn);
  VectorInit(&s->counts, sizeof(int), NULL);
}

typedef struct{
  FreeFn free_fn;
} DisposeAux;

void dispose_map_fn(int index, void* elem, void* aux){
  DisposeAux* my_aux = (DisposeAux*)aux;
  if(my_aux->free_fn == NULL) return;
  my_aux->free_fn(elem);
}

void SortedMultiSetDestroy(SortedMultiSet* s) {
  // IMPLEMENT
  VectorDestroy(&s->counts);
  //free(&s->counts);
  DisposeAux aux;
  aux.free_fn = s->free_fn;
  VectorMap(&s->v, dispose_map_fn, &aux);
  //free(&s->v);
}

typedef struct{
  void* result;
  void* target_elem;
  CmpFn cmp_fn;
  int elem_size;
  int found_idx;
} GetAux2;

void get_map_fn_2(int index, void* elem, void* aux){
  GetAux2* my_aux = (GetAux2*)aux;
  if(my_aux->cmp_fn(elem, my_aux->target_elem)==0){
    my_aux->found_idx = index;
    memcpy(&my_aux->result, elem, my_aux->elem_size);
  }
}

typedef struct{
  void* result;
  int target_index;
} GetAux3;

/*
  | 3 | 5 | ...
  elem -> ptr to 3 = 0x932859
  result -> ptr to 3 = 0x932859
*/
void get_map_fn_3(int index, void* elem, void* aux){
  GetAux3* my_aux = (GetAux3*)aux;
  if(index == my_aux->target_index){
    my_aux->result = elem;
  }
}

typedef struct{
  int result;
  void* to_add;
  CmpFn cmp_fn;
} AuxFirstLarger;

void find_first_larger_map_fn(int index, void* elem, void* aux){
  AuxFirstLarger* my_aux = (AuxFirstLarger*)aux;
  if(my_aux->result != -1) return;
  if(my_aux->cmp_fn(elem, my_aux->to_add) > 0){
    my_aux->result = index;
  }
}

int SortedMultiSetInsert(SortedMultiSet* s, void* elem) {
  // IMPLEMENT
  GetAux2 get_aux;
  get_aux.target_elem = elem;
  void* result = NULL;
  get_aux.result = result;
  get_aux.cmp_fn = s->cmp_fn;
  get_aux.elem_size = s->elem_size;
  get_aux.found_idx = -1;

  VectorMap(&s->v, get_map_fn_2, &get_aux);

  if(get_aux.result == NULL){
    AuxFirstLarger aux_first_larger;
    aux_first_larger.result = -1;
    aux_first_larger.cmp_fn = s->cmp_fn;
    aux_first_larger.to_add = elem;
    VectorMap(&s->v, find_first_larger_map_fn, &aux_first_larger);
    
    if(aux_first_larger.result == -1) aux_first_larger.result = VectorSize(&s->v);
    VectorInsert(&s->v, aux_first_larger.result, elem);
    int newCount = 1;
    VectorInsert(&s->counts, aux_first_larger.result, &newCount);

    return aux_first_larger.result;
  }else{
    GetAux3 get_aux_3;
    get_aux_3.result = NULL;
    get_aux_3.target_index = get_aux.found_idx;
    VectorMap(&s->counts, get_map_fn_3, &get_aux_3);
    (*(int*)get_aux_3.result)++;
    return get_aux.found_idx;
  }
}

typedef struct{
  void* result;
  int target_index;
} GetAux;

/*
  | 1 | ptr | ...
  elem -> ptr to elem -> 0x99
*/
void get_map_fn(int index, void* elem, void* aux){
  //printf("CUR IDX: %d\n", index);
  GetAux* my_aux = (GetAux*)aux;
  if(index == my_aux->target_index){
    //printf("BEFORE MEMCPY\n");
    //printf("ELEM IS: %d\n", *(int*)elem);
    //printf("ADRESS OF ELEM: %p\n", elem);
    memcpy(&(my_aux->result), &elem, sizeof(void*));
    //printf("ADRESS OF ELEM2: %p\n", my_aux->result);
    //printf("AFTER MEMCPY\n");
  }
}

void* SortedMultiSetGet(SortedMultiSet* s, int index) {
  // IMPLEMENT
  assert(index < VectorSize(&s->v));

  GetAux get_aux;
  get_aux.target_index = index;
  void* result = NULL;
  get_aux.result = result;

  //printf("BEFORE MAP\n");
  VectorMap(&s->v, get_map_fn, &get_aux);
  //printf("AFTER MAP\n");
  //printf("ADRESS OF ELEM3: %p\n", get_aux.result);
  //printf("passed\n");
  //printf("FOUND: %d\n", *(int*)get_aux.result);

  return get_aux.result;
}


int SortedMultiSetGetCount(SortedMultiSet* s, void* elem) {
  GetAux2 get_aux;
  get_aux.target_elem = elem;
  void* result = NULL;
  get_aux.result = result;
  get_aux.cmp_fn = s->cmp_fn;
  get_aux.elem_size = s->elem_size;
  get_aux.found_idx = -1;
  VectorMap(&s->v, get_map_fn_2, &get_aux);

  if(get_aux.found_idx == -1) return 0; 
  
  GetAux3 get_aux_3;
  get_aux_3.result = NULL;
  get_aux_3.target_index = get_aux.found_idx;
  VectorMap(&s->counts, get_map_fn_3, &get_aux_3);

  //printf("FOUND RESULT: %d\n", *(int*)get_aux_3.result);

  return *(int*)get_aux_3.result;
}
