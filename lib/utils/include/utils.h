// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#ifndef UTILS_H
#define UTILS_H

#include "errors.h"

err_t utils_copy_string(char** dest, const char* src, unsigned int size);

err_t utils_join_strings(char** dest, const char* str1, const char* str2, const char* sep);

err_t utils_string_filter(char* str, const char* chars_to_remove);

#endif  // UTILS_H
