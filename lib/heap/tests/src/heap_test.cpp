// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#include <gtest/gtest.h>

extern "C" {
#include "heap.h"
}

heap_t* heap_create_test(int n) {
  heap_t* heap = heap_create(n);
  EXPECT_EQ(heap == NULL, false);
  EXPECT_EQ(heap->size, 0);
  EXPECT_EQ(heap->capacity, n);
  EXPECT_EQ(heap->nodes == NULL, false);
  return heap;
}

void heap_insert_test(heap_t* heap, int n) {
  for (int i = 1; i <= n; ++i) {
    heap_insert(heap, {i, i});
  }
}

TEST(heap_t, heap_create) {
  heap_t* heap = NULL;
  heap_free(heap);

  heap = heap_create_test(42);
  EXPECT_TRUE(heap != NULL);

  heap_free(heap);
}

TEST(heap_t, heap_insert) {
  int n = 10;
  heap_t* heap = heap_create_test(n);
  EXPECT_TRUE(heap != NULL);

  heap_insert_test(heap, 2 * n);

  EXPECT_EQ(heap->size, n);
  for (int i = n + 1; i <= 2 * n; ++i) {
    EXPECT_EQ(heap_get_root(heap).value, i);
    EXPECT_EQ(heap_extract_root(heap).value, i);
  }

  heap_free(heap);
}
