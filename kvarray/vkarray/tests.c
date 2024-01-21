#include "sorted_kv_array.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT(expr)                                                           \
  {                                                                            \
    if (!(expr)) {                                                             \
      printf("%s:%d: Assertion failed: %s\n", __FILE__, __LINE__, #expr);      \
      return false;                                                            \
    }                                                                          \
  }

#define TEST(name) bool Test##name()

#define RUN_TEST(name)                                                         \
  printf("++ RUNNING TEST: %s\n", #name);                                      \
  if (Test##name()) {                                                          \
    printf("-- TEST %s: SUCCEEDED\n", #name);                                  \
  } else {                                                                     \
    printf("-- TEST %s: FAILED\n", #name);                                     \
  }                                                                            \
  printf("\n");

int IntCmp(void *a, void *b) {
  return *(int *)a - *(int *)b;
}

int StrCmpDyn(void *a, void *b) {
  return strcmp(*(char **)a, *(char **)b);
}

void StrFree(void *ptr) { free(*(char **)ptr); }

typedef struct {
  int x;
  int y;
} Point;

void PointFree(void *ptr) { free(*(Point **)ptr); }

Point PointNew(int x, int y) {
  Point p;
  p.x = x;
  p.y = y;
  return p;
}

Point *PointNewDyn(int x, int y) {
  Point *p = malloc(sizeof(Point));
  assert(p != NULL);
  p->x = x;
  p->y = y;
  return p;
}

int PointCmp(void *a, void *b) {
  const Point *p = a;
  const Point *q = b;
  if (p->x != q->x) {
    return p->x - q->x;
  }
  return p->y - q->y;
}

int PointCmpDyn(void *a, void *b) {
  return PointCmp(*(Point **)a, *(Point **)b);
}

TEST(Integers_Empty) {
  SortedKVArray l;
  SortedKVArrayInit(&l, sizeof(int), sizeof(int), IntCmp, NULL, NULL);
  for (int i = 0; i < 100; ++i) {
    ASSERT(SortedKVArrayGetValue(&l, &i) == NULL);
    ASSERT(SortedKVArrayRemove(&l, &i) == false);
  }
  SortedKVArrayDestroy(&l);
  return true;
}

TEST(Integers_SingleKey) {
  SortedKVArray l;
  SortedKVArrayInit(&l, sizeof(int), sizeof(int), IntCmp, NULL, NULL);
  int x = 5;
  int y = 7;
  ASSERT(SortedKVArrayGetValue(&l, &x) == NULL);
  SortedKVArrayAdd(&l, &x, &y);
  ASSERT(*(const int *)SortedKVArrayGetValue(&l, &x) == 7);
  y = 10;
  SortedKVArrayAdd(&l, &x, &y);
  ASSERT(*(const int *)SortedKVArrayGetValue(&l, &x) == 10);
  ASSERT(SortedKVArrayRemove(&l, &x));
  ASSERT(SortedKVArrayGetValue(&l, &x) == NULL);
  ASSERT(SortedKVArrayRemove(&l, &x) == false);
  SortedKVArrayDestroy(&l);
  return true;
}

TEST(Integers_AddGetRemove) {
  SortedKVArray l;
  SortedKVArrayInit(&l, sizeof(int), sizeof(int), IntCmp, NULL, NULL);
  for (int i = 99; i >= 0; --i) {
    SortedKVArrayAdd(&l, &i, &i);
  }
  for (int i = 0; i < 100; ++i) {
    ASSERT(*(const int *)SortedKVArrayGetValue(&l, &i) == i);
  }
  for (int i = 0; i < 100; ++i) {
    ASSERT(SortedKVArrayRemove(&l, &i));
  }
  for (int i = 0; i < 100; ++i) {
    ASSERT(SortedKVArrayGetValue(&l, &i) == NULL);
  }
  for (int i = 0; i < 100; ++i) {
    ASSERT(SortedKVArrayRemove(&l, &i) == false);
  }
  SortedKVArrayDestroy(&l);
  return true;
}

TEST(Integers_Duplicates) {
  SortedKVArray l;
  SortedKVArrayInit(&l, sizeof(int), sizeof(int), IntCmp, NULL, NULL);
  for (int i = 99; i >= 0; --i) {
    SortedKVArrayAdd(&l, &i, &i);
  }
  for (int i = 0; i < 100; ++i) {
    ASSERT(*(const int *)SortedKVArrayGetValue(&l, &i) == i);
  }
  for (int i = 0; i < 100; ++i) {
    int j = 100 - i;
    SortedKVArrayAdd(&l, &i, &j);
  }
  for (int i = 0; i < 100; ++i) {
    ASSERT(*(const int *)SortedKVArrayGetValue(&l, &i) == 100 - i);
  }
  SortedKVArrayDestroy(&l);
  return true;
}

TEST(IntegerPoint_SingleKey) {
  SortedKVArray l;
  SortedKVArrayInit(&l, sizeof(int), sizeof(Point), IntCmp, NULL, NULL);
  int x = 5;
  Point p = PointNew(1, 2);
  SortedKVArrayAdd(&l, &x, &p);
  ASSERT(PointCmp(SortedKVArrayGetValue(&l, &x), &p) == 0);
  ASSERT(SortedKVArrayRemove(&l, &x));
  ASSERT(SortedKVArrayGetValue(&l, &x) == NULL);
  SortedKVArrayAdd(&l, &x, &p);
  SortedKVArrayDestroy(&l);
  return true;
}

TEST(IntegerPoint_AddGetRemove) {
  SortedKVArray l;
  SortedKVArrayInit(&l, sizeof(int), sizeof(Point), IntCmp, NULL, NULL);
  for (int i = 99; i >= 0; --i) {
    Point p = PointNew(i, 100 - i);
    SortedKVArrayAdd(&l, &i, &p);
  }
  for (int i = 0; i < 100; ++i) {
    Point p = PointNew(i, 100 - i);
    ASSERT(PointCmp(SortedKVArrayGetValue(&l, &i), &p) == 0);
  }
  for (int i = 0; i < 100; ++i) {
    ASSERT(SortedKVArrayRemove(&l, &i));
  }
  for (int i = 0; i < 100; ++i) {
    ASSERT(SortedKVArrayGetValue(&l, &i) == NULL);
  }
  SortedKVArrayDestroy(&l);
  return true;
}

TEST(IntegerPoint_Duplicates) {
  SortedKVArray l;
  SortedKVArrayInit(&l, sizeof(int), sizeof(Point), IntCmp, NULL, NULL);
  for (int i = 99; i >= 0; --i) {
    Point p = PointNew(i, 100 - i);
    SortedKVArrayAdd(&l, &i, &p);
  }
  for (int i = 0; i < 100; ++i) {
    Point p = PointNew(100 - i, i);
    SortedKVArrayAdd(&l, &i, &p);
  }
  for (int i = 0; i < 100; ++i) {
    Point p = PointNew(100 - i, i);
    ASSERT(PointCmp(SortedKVArrayGetValue(&l, &i), &p) == 0);
  }
  SortedKVArrayDestroy(&l);
  return true;
}

TEST(IntegerPoint_ValueDynamicallyAllocated) {
  SortedKVArray l;
  SortedKVArrayInit(&l, sizeof(int), sizeof(Point *), IntCmp, NULL, PointFree);
  for (int i = 0; i < 100; ++i) {
    Point *p = PointNewDyn(i, 100 - i);
    SortedKVArrayAdd(&l, &i, &p);
  }
  for (int i = 0; i < 100; ++i) {
    Point p = PointNew(i, 100 - i);
    ASSERT(PointCmp(*(Point **)SortedKVArrayGetValue(&l, &i), &p) == 0);
  }
  for (int i = 0; i < 50; ++i) {
    ASSERT(SortedKVArrayRemove(&l, &i));
    ASSERT(SortedKVArrayGetValue(&l, &i) == NULL);
  }
  SortedKVArrayDestroy(&l);
  return true;
}

TEST(PointInteger_SingleKey) {
  SortedKVArray l;
  SortedKVArrayInit(&l, sizeof(Point), sizeof(int), IntCmp, NULL, NULL);
  int x = 5;
  Point p = PointNew(1, 2);
  SortedKVArrayAdd(&l, &p, &x);
  ASSERT(*(const int *)SortedKVArrayGetValue(&l, &p) == x);
  ASSERT(SortedKVArrayRemove(&l, &p));
  ASSERT(SortedKVArrayGetValue(&l, &p) == NULL);
  SortedKVArrayAdd(&l, &p, &x);
  SortedKVArrayDestroy(&l);
  return true;
}

TEST(PointInteger_AddGetRemove) {
  SortedKVArray l;
  SortedKVArrayInit(&l, sizeof(Point), sizeof(int), IntCmp, NULL, NULL);
  for (int i = 99; i >= 0; --i) {
    Point p = PointNew(i, 100 - i);
    SortedKVArrayAdd(&l, &p, &i);
  }
  for (int i = 0; i < 100; ++i) {
    Point p = PointNew(i, 100 - i);
    ASSERT(*(const int *)SortedKVArrayGetValue(&l, &p) == i);
  }
  for (int i = 0; i < 100; ++i) {
    Point p = PointNew(i, 100 - i);
    ASSERT(SortedKVArrayRemove(&l, &p));
  }
  for (int i = 0; i < 100; ++i) {
    Point p = PointNew(i, 100 - i);
    ASSERT(SortedKVArrayGetValue(&l, &p) == NULL);
  }
  SortedKVArrayDestroy(&l);
  return true;
}

TEST(PointInteger_Duplicates) {
  SortedKVArray l;
  SortedKVArrayInit(&l, sizeof(Point), sizeof(int), IntCmp, NULL, NULL);
  for (int i = 99; i >= 0; --i) {
    Point p = PointNew(i, 100 - i);
    SortedKVArrayAdd(&l, &p, &i);
  }
  for (int i = 0; i < 100; ++i) {
    Point p = PointNew(100 - i, i);
    SortedKVArrayAdd(&l, &p, &i);
  }
  for (int i = 0; i < 100; ++i) {
    Point p = PointNew(100 - i, i);
    ASSERT(*(const int *)SortedKVArrayGetValue(&l, &p) == i);
  }
  SortedKVArrayDestroy(&l);
  return true;
}

TEST(PointInteger_KeyDynamicallyAllocated) {
  SortedKVArray l;
  SortedKVArrayInit(&l, sizeof(Point *), sizeof(int), PointCmpDyn, PointFree,
                 NULL);
  for (int i = 0; i < 100; ++i) {
    Point *p = PointNewDyn(i, 100 - i);
    SortedKVArrayAdd(&l, &p, &i);
  }
  for (int i = 0; i < 100; ++i) {
    Point *p = PointNewDyn(i, 100 - i);
    ASSERT(*(const int *)SortedKVArrayGetValue(&l, &p) == i);
    free(p);
  }
  for (int i = 0; i < 50; ++i) {
    Point *p = PointNewDyn(i, 100 - i);
    ASSERT(SortedKVArrayRemove(&l, &p));
    ASSERT(SortedKVArrayGetValue(&l, &p) == NULL);
    free(p);
  }
  SortedKVArrayDestroy(&l);
  return true;
}

char *IntToStr(int n) {
  char *ret = malloc(5);
  assert(ret != NULL);
  int len = 0;
  while (n > 0) {
    ret[len] = n % 10 + 1;
    len++;
    n /= 10;
  }
  ret[len] = '\0';
  for (int i = 0; 2 * i < n; ++i) {
    char tmp = ret[i];
    ret[i] = ret[len - i - 1];
    ret[len - i - 1] = tmp;
  }
  return ret;
}

TEST(StringPoint_BothKeyValueDynamic) {
  SortedKVArray l;
  SortedKVArrayInit(&l, sizeof(char *), sizeof(Point *), StrCmpDyn, StrFree,
                 PointFree);
  for (int i = 99; i >= 0; --i) {
    char *i_str = IntToStr(i);
    Point *p = PointNewDyn(i, 100 - i);
    SortedKVArrayAdd(&l, &i_str, &p);
  }
  for (int i = 0; i < 100; ++i) {
    char *i_str = IntToStr(i);
    Point *p = PointNewDyn(100 - i, i);
    SortedKVArrayAdd(&l, &i_str, &p);
  }
  for (int i = 0; i < 100; ++i) {
    char *i_str = IntToStr(i);
    Point p = PointNew(100 - i, i);
    const Point *q = SortedKVArrayGetValue(&l, &i_str);
    ASSERT(PointCmp(*(Point **)SortedKVArrayGetValue(&l, &i_str), &p) == 0);
    free(i_str);
  }
  for (int i = 0; i < 50; ++i) {
    char *i_str = IntToStr(i);
    ASSERT(SortedKVArrayRemove(&l, &i_str));
    ASSERT(SortedKVArrayGetValue(&l, &i_str) == NULL);
    free(i_str);
  }
  SortedKVArrayDestroy(&l);
  return true;
}

int main(int argc, char **argv) {
  RUN_TEST(Integers_Empty);
  RUN_TEST(Integers_SingleKey);
  RUN_TEST(Integers_AddGetRemove);
  RUN_TEST(Integers_Duplicates);
  RUN_TEST(IntegerPoint_SingleKey);
  RUN_TEST(IntegerPoint_AddGetRemove);
  RUN_TEST(IntegerPoint_Duplicates);
  RUN_TEST(IntegerPoint_ValueDynamicallyAllocated);
  RUN_TEST(PointInteger_SingleKey);
  RUN_TEST(PointInteger_AddGetRemove);
  RUN_TEST(PointInteger_Duplicates);
  RUN_TEST(PointInteger_KeyDynamicallyAllocated);
  RUN_TEST(StringPoint_BothKeyValueDynamic);
  return 0;
}
