// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"

// ----------------------------- PRIVATE ----------------------------- //

void remove_all_chars(char* str, char c) {
  char* head = str;
  char* tail = str;
  while (*tail) {
    *head = *tail++;
    if (*head != c) {
      ++head;
    }
  }
  *head = '\0';
}

void to_lower(char* str) {
  int delta = 'A' - 'a';
  for (size_t i = 0; i < strlen(str); ++i) {
    if (str[i] >= 'A' && str[i] <= 'Z') {
      str[i] -= delta;
    }
  }
}

// ----------------------------- PUBLIC ----------------------------- //

err_t utils_copy_string(char** dest, const char* src, unsigned int size) {
  if (dest == NULL || src == NULL) {
    return ERR_NULL_PTR_REFERENCE;
  }

  char* buf = realloc(*dest, size + 1);
  if (buf == NULL) {
    return ERR_MEMORY_ALLOCATION;
  }

  memcpy(buf, src, size);
  buf[size] = '\0';
  *dest = buf;

  return ERR_NIL;
}

err_t utils_join_strings(char** dest, const char* str1, const char* str2, const char* sep) {
  if (dest == NULL || str1 == NULL || str2 == NULL || sep == NULL) {
    return ERR_NULL_PTR_REFERENCE;
  }

  unsigned int size = sizeof(str1) + sizeof(str2) + sizeof(sep);
  char* buf = realloc(*dest, size + 1);
  if (buf == NULL) {
    return ERR_MEMORY_ALLOCATION;
  }

  snprintf(buf, size, "%s%s%s", str1, sep, str2);
  buf[size] = '\0';
  *dest = buf;

  return ERR_NIL;
}

err_t utils_string_filter(char* str, const char* chars_to_remove) {
  if (!str) {
    return ERR_NULL_PTR_REFERENCE;
  }

  for (size_t i = 0; i < strlen(chars_to_remove); ++i) {
    remove_all_chars(str, chars_to_remove[i]);
  }

  to_lower(str);

  return ERR_NIL;
}
