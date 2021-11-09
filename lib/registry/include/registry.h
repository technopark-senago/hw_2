// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#ifndef REGISTRY_H
#define REGISTRY_H

#include <dirent.h>

#include "constants.h"
#include "errors.h"

typedef struct registry_t {
  DIR* dir;
  char** filenames;
  size_t number_of_files;
} registry_t;

registry_t* registry_create();

err_t registry_scan_dir(registry_t* registry, const char* dir_path);

void registry_free(registry_t* registry);

#endif  // REGISTRY_H
