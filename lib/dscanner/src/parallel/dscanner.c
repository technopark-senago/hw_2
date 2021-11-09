// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#include "dscanner.h"

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "constants.h"
#include "fscanner.h"
#include "heap.h"
#include "registry.h"
#include "utils.h"

// ----------------------------- PRIVATE ----------------------------- //

void dscanner_init(dscanner_t* dscanner) {
  dscanner->heap = NULL;
  dscanner->registry = NULL;
  dscanner->fds = NULL;
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

  int amount_of_cores = sysconf(_SC_NPROCESSORS_ONLN);
  dscanner->fds = calloc(2 * amount_of_cores, sizeof(*dscanner->fds));
  if (dscanner->fds == NULL) {
    return ERR_MEMORY_ALLOCATION;
  }
  dscanner->number_of_processes = amount_of_cores;

  return ERR_NIL;
}

err_t dscanner_create_pipes(dscanner_t* dscanner) {
  for (size_t i = 0; i < dscanner->number_of_processes; ++i) {
    if (pipe(&dscanner->fds[2 * i]) == -1) {
      return ERR_OPEN_PIPE;
    }
  }
  return ERR_NIL;
}

void dscanner_run_child_job(dscanner_t* dscanner, const char* dir_path, const char* word, size_t k) {
  close(dscanner->fds[2 * k]);

  fscanner_t* fscanner = fscanner_create();
  if (fscanner == NULL) {
    exit(ERR_MEMORY_ALLOCATION);
  }

  size_t index = k;
  char* filepath = NULL;
  while (index < dscanner->registry->number_of_files) {
    err_t err = utils_join_strings(&filepath, dir_path, dscanner->registry->filenames[index], "/");
    if (err != ERR_NIL) {
      break;
    }

    err = fscanner_count(fscanner, filepath, word);
    if (err == ERR_NIL) {
      node_t node = {index, fscanner->count};
      if (write(dscanner->fds[2 * k + 1], &node, sizeof(node)) == -1) {
        break;
      }
    }

    index += dscanner->number_of_processes;
  }

  free(filepath);
  fscanner_free(fscanner);
  close(dscanner->fds[2 * k + 1]);

  exit(0);
}

err_t dscanner_run_processes(dscanner_t* dscanner, const char* dir_path, const char* word) {
  for (size_t i = 0; i < dscanner->number_of_processes; ++i) {
    pid_t pid = fork();
    if (pid == -1) {
      return ERR_FORK_FAILED;
    }

    if (pid == 0) {
      dscanner_run_child_job(dscanner, dir_path, word, i);
    } else {
      close(dscanner->fds[2 * i + 1]);
    }
  }

  // wait for all children to finish
  wait(NULL);

  // process messages from children
  for (size_t i = 0; i < dscanner->number_of_processes; ++i) {
    node_t node;
    while (read(dscanner->fds[2 * i], &node, sizeof(node)) > 0) {
      heap_insert(dscanner->heap, node);
    }
    close(dscanner->fds[2 * i]);
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

  err = dscanner_create_pipes(dscanner);
  if (err != ERR_NIL) {
    return err;
  }

  err = dscanner_run_processes(dscanner, dir_path, word);
  if (err != ERR_NIL) {
    return err;
  }

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

  if (dscanner->fds) {
    free(dscanner->fds);
  }

  free(dscanner);
}
