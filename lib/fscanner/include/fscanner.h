// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#ifndef FSCANNER_H
#define FSCANNER_H

#include <stdio.h>

#include "errors.h"

typedef struct fscanner_t {
  FILE* fptr;
  char* buffer;
  unsigned int count;
} fscanner_t;

fscanner_t* fscanner_create();

err_t fscanner_count(fscanner_t* fw, const char* filepath, const char* word);

void fscanner_free(fscanner_t* fw);

#endif  // FSCANNER_H
