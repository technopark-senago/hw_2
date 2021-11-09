// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#include <gtest/gtest.h>

#include <algorithm>
#include <string>

extern "C" {
#include "registry.h"
}

constexpr char TEST_DIR[] = "static/data";
const std::string TEST_DIR_FILES[] = {"test.rtf", "test.txt"};

TEST(registry_t, registry_create) {
  registry_t* registry = NULL;
  registry_free(registry);

  registry = registry_create();
  EXPECT_TRUE(registry != NULL);

  registry_free(registry);
}

TEST(registry_t, registry_scan_dir) {
  registry_t* registry = registry_create();
  EXPECT_TRUE(registry != NULL);

  err_t err = registry_scan_dir(registry, "invalid");
  EXPECT_TRUE(err != ERR_NIL);

  err = registry_scan_dir(registry, TEST_DIR);
  EXPECT_EQ(err, ERR_NIL);

  size_t size = sizeof(TEST_DIR_FILES) / sizeof(TEST_DIR_FILES[0]);
  EXPECT_EQ(size, registry->number_of_files);

  for (size_t i = 0; i < size; ++i) {
    bool found = std::end(TEST_DIR_FILES) !=
                 std::find(std::begin(TEST_DIR_FILES), std::end(TEST_DIR_FILES), registry->filenames[i]);
    EXPECT_TRUE(found);
  }

  registry_free(registry);
}
