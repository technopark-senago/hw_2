// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#include "fscanner.h"

#include <string.h>

#include "constants.h"
#include "utils.h"

// ----------------------------- PRIVATE ----------------------------- //

static const char* FMODE_READ = "r";

void fscanner_init(fscanner_t* fscanner) {
  fscanner->fptr = NULL;
  fscanner->buffer = calloc(MAX_WORD_LENGTH, sizeof(fscanner->buffer));
  fscanner->count = 0;
}

void fscanner_reset(fscanner_t* fscanner) {
  if (fscanner->fptr) {
    fclose(fscanner->fptr);
  }
  fscanner->count = 0;
}

err_t fscanner_open_file(fscanner_t* fscanner, const char* filepath) {
  fscanner->fptr = fopen(filepath, FMODE_READ);
  if (fscanner->fptr == NULL) {
    return ERR_OPEN_FILE;
  }
  return ERR_NIL;
}

err_t fscanner_read(fscanner_t* fscanner) {
  int code = fscanf(fscanner->fptr, "%1023s", fscanner->buffer);
  if (code != 1) {
    return ERR_EOF;
  }
  utils_string_filter(fscanner->buffer, CHARS_TO_REMOVE);
  return ERR_NIL;
}

// ----------------------------- PUBLIC ----------------------------- //

fscanner_t* fscanner_create() {
  fscanner_t* fscanner = malloc(sizeof(fscanner_t));
  if (fscanner) {
    fscanner_init(fscanner);
  }
  return fscanner;
}

err_t fscanner_count(fscanner_t* fscanner, const char* filepath, const char* word) {
  if (fscanner == NULL) {
    return ERR_NULL_PTR_REFERENCE;
  }

  fscanner_reset(fscanner);
  err_t err = fscanner_open_file(fscanner, filepath);
  if (err != ERR_NIL) {
    return err;
  }

  while (fscanner_read(fscanner) == ERR_NIL) {
    if (!strcmp(fscanner->buffer, word)) {
      fscanner->count += 1;
    }
  }

  return ERR_NIL;
}

void fscanner_free(fscanner_t* fscanner) {
  if (fscanner == NULL) {
    return;
  }

  if (fscanner->fptr) {
    fclose(fscanner->fptr);
  }

  if (fscanner->buffer) {
    free(fscanner->buffer);
  }

  free(fscanner);
}
