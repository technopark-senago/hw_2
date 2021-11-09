// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>

typedef struct node_t {
  int key;
  int value;
} node_t;

typedef struct heap_t {
  size_t size;
  size_t capacity;
  node_t* nodes;
} heap_t;

heap_t* heap_create(size_t n);

void heap_insert(heap_t* heap, node_t node);

node_t heap_get_root(heap_t* heap);

node_t heap_extract_root(heap_t* heap);

void heap_free(heap_t* heap);

#endif  // HEAP_H
