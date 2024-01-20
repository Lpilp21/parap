#include "array2d.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>



typedef struct{
  void* elem;
  bool is_empty;
}Elem;

// (3 5)  10x10 2*10 + 5
#define get(i, j) ((char*)arr->base + (i)*arr->num_cols*sizeof(Elem) + j*sizeof(Elem))

void Array2DInit(Array2D* arr, int n, int m, int elem_size, FreeFn free_fn) {
  arr->elem_size = elem_size;
  arr->num_rows = n;
  arr->num_cols = m;
  arr->free_fn = free_fn;
  arr->base = malloc(n*m*sizeof(Elem));
  //printf("Base Address: %p\n", arr->base);
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      ((Elem*)get(i, j))->is_empty = true;
      ((Elem*)get(i, j))->elem = NULL;
    }
  }
}

void Array2DDestroy(Array2D* arr) {
  if(arr->free_fn == NULL) return;
  for(int i = 0; i < arr->num_rows; i++){
    for(int j = 0; j < arr->num_cols; j++){
      if(((Elem*)get(i, j))->is_empty) continue;
      arr->free_fn(((Elem*)get(i, j))->elem);
    }
  }
  free(arr->base);
}

int Array2DN(Array2D* arr) {
  return arr->num_rows;
}

int Array2DM(Array2D* arr) {
  return arr->num_cols;
}

void Array2DSet(Array2D* arr, int i, int j, void* value) {
   if(((Elem *)get(i, j))->is_empty) ((Elem*)get(i, j))->elem = malloc(arr->elem_size);
   
   memcpy(((Elem*)get(i, j))->elem, value, arr->elem_size);
   ((Elem *)get(i, j))->is_empty = false; 
}

const void* Array2DGet(Array2D* arr, int i, int j) {
  //printf("Index in get: %d\n", arr->num_cols*i + j);
  return ((Elem*)get(i, j))->elem;
}

void Array2DRemove(Array2D* arr, int i, int j) {
 free(((Elem*)get(i, j))->elem);
 ((Elem *)get(i, j))->is_empty = true;
}

void Array2DIterate(Array2D* arr, int x, int y, int w, int z, IterFn iter_fn, void* aux) {
  for(int i = x; i <= w; i++){
    for(int j = y; j <=z; j++){
     // printf("Indexes: %d %d\n", i, j);
      //printf("Pointer: %p \n", (Elem *)get(i, j));
      //printf("elemsize: %ld\n",sizeof(Elem));
      //printf("Value: %d\n", *((bool*)((Elem *)get(i, j))));
      if(((Elem *)get(i, j))->is_empty) continue;
     //printf("after if\n");
      //printf("get ret value: %d \n", *(bool*)Array2DGet(arr, i, j));
      iter_fn(aux, Array2DGet(arr, i, j), i, j);
      //printf("finished iterating\n");
    }
  }
}

void Array2DGrow(Array2D* arr, int n, int m) {
 void* temp = malloc(n * m * sizeof(Elem));
 
 for(int i = 0; i < arr->num_rows; i++){
  for(int j = 0; j < arr->num_cols; j++){
    memcpy(
      (char*)temp + sizeof(Elem)*(i*m + j),
      get(i,j),
      sizeof(Elem)
    );
  }
 }
 for(int i = arr->num_rows; i < n; i++){
  for(int j = arr->num_cols; j < m; j++){
    ((Elem *)((char*)temp + sizeof(Elem)*(i*m + j)))->elem = NULL;
    ((Elem *)((char*)temp + sizeof(Elem)*(i*m + j)))->is_empty = true;
  }
 }
  for(int i = 0; i < n; i++){
    for(int j = arr->num_cols; j < m; j++){
      ((Elem *)((char*)temp + sizeof(Elem)*(i*m + j)))->elem = NULL;
      ((Elem *)((char*)temp + sizeof(Elem)*(i*m + j)))->is_empty = true;
    }
  }

  for(int i = arr->num_rows; i < n; i++){
    for(int j = 0; j < m; j++){
      ((Elem *)((char*)temp + sizeof(Elem)*(i*m + j)))->elem = NULL;
      ((Elem *)((char*)temp + sizeof(Elem)*(i*m + j)))->is_empty = true;
    }
  }
 free(arr->base);
 arr->base = temp;
 arr->num_cols = m;
 arr->num_rows = n;
}
