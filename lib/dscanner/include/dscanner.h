// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#ifndef DSCANNER_H
#define DSCANNER_H

#include "errors.h"
#include "heap.h"
#include "registry.h"

typedef struct dscanner_t {
  heap_t* heap;
  registry_t* registry;

  int* fds;
  size_t number_of_processes;
} dscanner_t;

dscanner_t* dscanner_create();

err_t dscanner_scan(dscanner_t* dscanner, const char* dir_path, const char* word);

err_t dscanner_print_results(dscanner_t* dscanner);

void dscanner_free(dscanner_t* dscanner);

#endif  // DSCANNER_H
