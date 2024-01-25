#include "vector.h"
#include "sorted_set.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ASSERT_INT_EQ(a, b) {						\
    int expected = (a);							\
    int actual = (b);							\
    if (expected != actual) {						\
      printf("%s:%d Assertion failed: expected %d got %d\n", __FILE__, __LINE__, expected, actual); \
      return false;							\
    }									\
  }

#define ASSERT_STR_EQ(a, b) {						\
    char* expected = (a);							\
    char* actual = (b);							\
    if (strcmp(expected, actual) != 0) {								\
      printf("%s:%d Assertion failed: expected %s got %s\n", __FILE__, __LINE__, expected, actual); \
      return false;							\
    }									\
  }


#define TEST(t) { \
    printf("-- TESTING: %s\n", #t); \
    if (t()) {	  \
      printf("TEST %s: SUCCEEDED\n", #t);	\
    } \
    else { \
      printf("TEST %s: FAILED\n", #t);		\
    } \
    printf("####\n\n"); \
  }

// Integers

int IntCmp(void* a, void* b) {
  return (*(int*)a) - (*(int*)b);
}

bool Empty() {
  SortedSet s;
  SortedSetInit(&s, sizeof(int), IntCmp, NULL);
  ASSERT_INT_EQ(0, SortedSetSize(&s));
  SortedSetDestroy(&s);
  return true;
}

bool Ints_OneToTen() {
  SortedSet s;
  SortedSetInit(&s, sizeof(int), IntCmp, NULL);
  for (int i = 1; i <= 10; i++) {
	ASSERT_INT_EQ(i - 1, SortedSetInsert(&s, &i));
	ASSERT_INT_EQ(i, SortedSetSize(&s));
	ASSERT_INT_EQ(i, *(int*)SortedSetGet(&s, i - 1));
  }
  for (int i = 0; i < 10; i++) {
	ASSERT_INT_EQ(i + 1, *(int*)SortedSetGet(&s, i));
  }
  SortedSetDestroy(&s);
  return true;
}

bool Ints_OneToTen_Duplicates() {
  SortedSet s;
  SortedSetInit(&s, sizeof(int), IntCmp, NULL);
  for (int i = 1; i <= 10; i++) {
	ASSERT_INT_EQ(i - 1, SortedSetInsert(&s, &i));
	ASSERT_INT_EQ(i, SortedSetSize(&s));
	ASSERT_INT_EQ(i, *(int*)SortedSetGet(&s, i - 1));
  }
  for (int i = 0; i < 10; i++) {
	ASSERT_INT_EQ(i + 1, *(int*)SortedSetGet(&s, i));
  }
  for (int i = 1; i <= 10; i++) {
	ASSERT_INT_EQ(-1, SortedSetInsert(&s, &i));
  }
  ASSERT_INT_EQ(10, SortedSetSize(&s));
  for (int i = 0; i < 10; i++) {
	ASSERT_INT_EQ(i + 1, *(int*)SortedSetGet(&s, i));
  }
  SortedSetDestroy(&s);
  return true;
}

bool Ints_OneThreeTwo() {
  SortedSet s;
  SortedSetInit(&s, sizeof(int), IntCmp, NULL);
  int one = 1;
  int two = 2;
  int three = 3;
  ASSERT_INT_EQ(0, SortedSetInsert(&s, &one));
  ASSERT_INT_EQ(1, SortedSetInsert(&s, &three));
  ASSERT_INT_EQ(1, SortedSetInsert(&s, &two));
  ASSERT_INT_EQ(3, SortedSetSize(&s));
  ASSERT_INT_EQ(1, *(int*)SortedSetGet(&s, 0));
  ASSERT_INT_EQ(2, *(int*)SortedSetGet(&s, 1));
  ASSERT_INT_EQ(3, *(int*)SortedSetGet(&s, 2));
  SortedSetDestroy(&s);
  return true;
}

// Strings

int StrCmp(void* a, void* b) {
  return strcmp(*(char**)a, *(char**)b);
}

void StrFree(void* a) {
  free(*(char**)a);
}

bool Strings_FooBarDevNull() {
  SortedSet s;
  SortedSetInit(&s, sizeof(char*), StrCmp, StrFree);
  char* a = strdup("foo");
  char* b = strdup("bar");
  char* c = strdup("dev");
  char* d = strdup("null");
  ASSERT_INT_EQ(0, SortedSetInsert(&s, &a));
  ASSERT_INT_EQ(0, SortedSetInsert(&s, &b));
  ASSERT_INT_EQ(1, SortedSetInsert(&s, &c));
  ASSERT_INT_EQ(3, SortedSetInsert(&s, &d));
  ASSERT_INT_EQ(4, SortedSetSize(&s));
  SortedSetDestroy(&s);
  return true;
}

int main(int argc, char** argv) {
  TEST(Empty);
  // Integers
  TEST(Ints_OneToTen);
  TEST(Ints_OneToTen_Duplicates);
  TEST(Ints_OneThreeTwo);
  // Strings
  TEST(Strings_FooBarDevNull);
  return 0;
}
