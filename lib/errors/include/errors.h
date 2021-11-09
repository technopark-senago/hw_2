// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#ifndef ERRORS_H
#define ERRORS_H

typedef enum err_t {
  ERR_NIL = 0,
  ERR_EOF,
  ERR_SEM_INIT,
  ERR_READ_DIR,
  ERR_OPEN_DIR,
  ERR_CLOSE_DIR,
  ERR_OPEN_PIPE,
  ERR_OPEN_FILE,
  ERR_CLOSE_PIPE,
  ERR_CLOSE_FILE,
  ERR_FORK_FAILED,
  ERR_CLOCK_GETTIME,
  ERR_INVALID_INPUT,
  ERR_MEMORY_ALLOCATION,
  ERR_NULL_PTR_REFERENCE,
} err_t;

#endif  // ERRORS_H
