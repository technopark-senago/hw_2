// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#include "heap.h"

#include <stdbool.h>

// an implementation of a fixed size max heap

// ----------------------------- PRIVATE ----------------------------- //

void heap_setup(heap_t* heap, size_t n) {
  heap->size = 0;
  heap->capacity = n;
  heap->nodes = calloc(n, sizeof(heap->nodes));
}

size_t parent(size_t k) { return (k - 1) / 2; }

size_t left_child(size_t k) { return 2 * k + 1; }

size_t right_child(size_t k) { return 2 * k + 2; }

// less actually makes the heap a min heap
// we need this to know if we should insert an element when it is out of capacity
bool less(heap_t* heap, size_t x, size_t y) { return heap->nodes[x].value > heap->nodes[y].value; }

void swap(heap_t* heap, size_t x, size_t y) {
  node_t tmp = heap->nodes[x];
  heap->nodes[x] = heap->nodes[y];
  heap->nodes[y] = tmp;
}

void swim(heap_t* heap, size_t k) {
  while (k != 0) {
    size_t p = parent(k);
    if (!less(heap, p, k)) {
      break;
    }
    swap(heap, p, k);
    k = p;
  }
}

void sink(heap_t* heap, size_t k) {
  size_t left = left_child(k);
  size_t right = right_child(k);
  size_t largest = k;

  if (left < heap->size && less(heap, largest, left)) {
    largest = left;
  }

  if (right < heap->size && less(heap, largest, right)) {
    largest = right;
  }

  if (largest != k) {
    swap(heap, largest, k);
    sink(heap, largest);
  }
}

// ----------------------------- PUBLIC ----------------------------- //

heap_t* heap_create(size_t n) {
  heap_t* heap = malloc(sizeof(heap_t));
  if (heap) {
    heap_setup(heap, n);
  }
  return heap;
}

void heap_insert(heap_t* heap, node_t node) {
  if (heap->size < heap->capacity) {
    heap->nodes[heap->size] = node;
    heap->size += 1;
    swim(heap, heap->size - 1);
  } else if (node.value > heap_get_root(heap).value) {
    heap_extract_root(heap);
    heap_insert(heap, node);
  }
}

node_t heap_get_root(heap_t* heap) { return heap->nodes[0]; }

node_t heap_extract_root(heap_t* heap) {
  node_t node = heap_get_root(heap);
  heap->nodes[0] = heap->nodes[heap->size - 1];
  heap->size -= 1;

  sink(heap, 0);
  return node;
}

void heap_free(heap_t* heap) {
  if (heap == NULL) {
    return;
  }

  if (heap->nodes) {
    free(heap->nodes);
  }

  free(heap);
}
