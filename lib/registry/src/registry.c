// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#include "registry.h"

#include <stdio.h>
#include <string.h>

#include "utils.h"

// ----------------------------- PRIVATE ----------------------------- //

void init_registry(registry_t* registry) {
  registry->dir = NULL;
  registry->filenames = NULL;
  registry->number_of_files = 0;
}

err_t registry_get_number_of_files(registry_t* registry, const char* dir_path) {
  DIR* dir = opendir(dir_path);
  if (dir == NULL) {
    return ERR_OPEN_DIR;
  }

  struct dirent* dirent = readdir(dir);
  unsigned int count = 0;
  while (dirent) {
    if (dirent->d_type == DT_REG) {
      ++count;
    }
    dirent = readdir(dir);
  }

  if (closedir(dir) == -1) {
    return ERR_CLOSE_DIR;
  }

  registry->number_of_files = count;

  return ERR_NIL;
}

err_t registry_open_dir(registry_t* registry, const char* dir_path) {
  registry->dir = opendir(dir_path);
  if (registry->dir == NULL) {
    return ERR_OPEN_DIR;
  }
  return ERR_NIL;
}

err_t registry_setup(registry_t* registry, const char* dir_path) {
  err_t err = registry_get_number_of_files(registry, dir_path);
  if (err != ERR_NIL) {
    return err;
  }

  registry->filenames = calloc(registry->number_of_files, sizeof(*registry->filenames));
  if (registry->filenames == NULL) {
    return ERR_MEMORY_ALLOCATION;
  }

  err = registry_open_dir(registry, dir_path);
  if (err != ERR_NIL) {
    return err;
  }

  return ERR_NIL;
}

err_t registry_seek_file(registry_t* registry, size_t k) {
  struct dirent* dirent = readdir(registry->dir);
  while (dirent && dirent->d_type != DT_REG) {
    dirent = readdir(registry->dir);
  }

  if (dirent) {
    err_t err = utils_copy_string(&registry->filenames[k], dirent->d_name, strlen(dirent->d_name));
    if (err != ERR_NIL) {
      return err;
    }
  }

  return ERR_NIL;
}

err_t registry_seek_files(registry_t* registry) {
  for (size_t i = 0; i < registry->number_of_files; ++i) {
    err_t err = registry_seek_file(registry, i);
    if (err != ERR_NIL) {
      return err;
    }
  }
  return ERR_NIL;
}

// ----------------------------- PUBLIC ----------------------------- //

registry_t* registry_create() {
  registry_t* registry = malloc(sizeof(registry_t));
  if (registry) {
    init_registry(registry);
  }
  return registry;
}

err_t registry_scan_dir(registry_t* registry, const char* dir_path) {
  if (registry == NULL) {
    return ERR_NULL_PTR_REFERENCE;
  }

  err_t err = registry_setup(registry, dir_path);
  if (err != ERR_NIL) {
    return err;
  }

  err = registry_seek_files(registry);
  if (err != ERR_NIL) {
    return err;
  }

  return ERR_NIL;
}

void registry_free(registry_t* registry) {
  if (registry == NULL) {
    return;
  }

  if (registry->dir) {
    closedir(registry->dir);
  }

  if (registry->filenames) {
    for (size_t i = 0; i < registry->number_of_files; ++i) {
      if (registry->filenames[i]) {
        free(registry->filenames[i]);
      }
    }
    free(registry->filenames);
  }

  free(registry);
}
