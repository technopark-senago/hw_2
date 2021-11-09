// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#include "dscanner.h"

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "fscanner.h"
#include "heap.h"
#include "registry.h"
#include "utils.h"

// ----------------------------- PRIVATE ----------------------------- //

void dscanner_init(dscanner_t* dscanner) {
  dscanner->heap = NULL;
  dscanner->registry = NULL;
}

err_t dscanner_setup_registry(dscanner_t* dscanner) {
  dscanner->registry = registry_create();
  if (dscanner->registry == NULL) {
    return ERR_MEMORY_ALLOCATION;
  }
  return ERR_NIL;
}

err_t dscanner_setup(dscanner_t* dscanner) {
  err_t err = dscanner_setup_registry(dscanner);
  if (err != ERR_NIL) {
    return err;
  }

  dscanner->heap = heap_create(MAX_TIER_LIST_SIZE);
  if (dscanner->heap == NULL) {
    return ERR_MEMORY_ALLOCATION;
  }

  return ERR_NIL;
}

bool dscanner_is_ready(dscanner_t* dscanner) {
  if (dscanner->heap && dscanner->registry) {
    return true;
  }
  return false;
}

// ----------------------------- PUBLIC ----------------------------- //

dscanner_t* dscanner_create() {
  dscanner_t* dscanner = malloc(sizeof(dscanner_t));
  if (dscanner) {
    dscanner_init(dscanner);
  }
  return dscanner;
}

err_t dscanner_scan(dscanner_t* dscanner, const char* dir_path, const char* word) {
  if (dscanner == NULL) {
    return ERR_NULL_PTR_REFERENCE;
  }

  if (!dscanner_is_ready(dscanner)) {
    err_t err = dscanner_setup(dscanner);
    if (err != ERR_NIL) {
      return err;
    }
  }

  err_t err = registry_scan_dir(dscanner->registry, dir_path);
  if (err != ERR_NIL) {
    return err;
  }

  fscanner_t* fscanner = fscanner_create();
  if (fscanner == NULL) {
    return ERR_MEMORY_ALLOCATION;
  }

  char* filepath = NULL;
  for (size_t i = 0; i < dscanner->registry->number_of_files; ++i) {
    err = utils_join_strings(&filepath, dir_path, dscanner->registry->filenames[i], "/");
    if (err != ERR_NIL) {
      break;
    }

    err = fscanner_count(fscanner, filepath, word);
    if (err == ERR_NIL) {
      node_t node = {i, fscanner->count};
      heap_insert(dscanner->heap, node);
    }
  }

  free(filepath);
  fscanner_free(fscanner);

  return ERR_NIL;
}

err_t dscanner_print_results(dscanner_t* dscanner) {
  if (dscanner == NULL || dscanner->heap == NULL) {
    return ERR_NULL_PTR_REFERENCE;
  }

  while (dscanner->heap->size != 0) {
    node_t node = heap_extract_root(dscanner->heap);
    printf("%s: %d\n", dscanner->registry->filenames[node.key], node.value);
  }

  return ERR_NIL;
}

void dscanner_free(dscanner_t* dscanner) {
  if (dscanner == NULL) {
    return;
  }

  if (dscanner->heap) {
    heap_free(dscanner->heap);
  }

  if (dscanner->registry) {
    registry_free(dscanner->registry);
  }

  free(dscanner);
}
