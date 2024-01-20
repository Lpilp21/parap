#include "str_array2d.h"

#include <stdlib.h>
#include <string.h>


void stringFreeFn(void* str){
  free(*(char**)str);
}

void StrArray2DInit(StrArray2D* arr, int n, int m) {
  Array2DInit(arr, n, m, sizeof(char*), stringFreeFn);
}

void StrArray2DDestroy(StrArray2D* arr) {
   Array2DDestroy(arr);
}

void StrArray2DSet(StrArray2D* arr, int i, int j, char* str) {
  char* tempStr = strdup(str);
  Array2DSet(arr, i, j, &tempStr);
}

const char* StrArray2DGet(StrArray2D* arr, int i, int j) {
  return *(char**)Array2DGet(arr, i, j);
}

typedef struct{
  bool found;
  const char* target_string;
  StrArray2D* arr;
}AuxData;

void iterate(void* aux, const void* elem, int i, int j){
  AuxData* currData = (AuxData*)aux;
  if(currData->found) return;
  if(strcmp(*(char**)elem, currData->target_string) == 0){
    Array2DRemove(currData->arr, i, j);
    currData->found = true;
  }
}


bool StrArray2DRemove(StrArray2D* arr, const char* str) {
  // IMPLEMENT
  // მხოლოდ შეგიძლიათ აღწეროთ ლოკალური ცვლადები,
  // გამოიძახოთ Array2DIterate ფუნქცია და დააბრუნოთ მნიშვნელობა.
  AuxData data;
  data.found = false;
  data.target_string = str;
  data.arr = arr;
  int numRows = Array2DN(arr);
  int numCols = Array2DM(arr);
  Array2DIterate(arr, 0, 0, numRows-1, numCols-1, iterate, &data);
  return data.found;
}
