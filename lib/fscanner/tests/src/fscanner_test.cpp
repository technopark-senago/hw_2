// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#include <gtest/gtest.h>

extern "C" {
#include "fscanner.h"
}

constexpr char TEST_FILE[] = "static/data/test.txt";
constexpr char TEST_WORD[] = "test";
constexpr unsigned int TEST_COUNT = 10;

TEST(fscanner_t, fscanner_create) {
  fscanner_t* fscanner = NULL;
  fscanner_free(fscanner);

  fscanner = fscanner_create();
  EXPECT_TRUE(fscanner != NULL);

  fscanner_free(fscanner);
}

TEST(fscanner_t, fscanner_count) {
  fscanner_t* fscanner = NULL;

  err_t err = fscanner_count(fscanner, TEST_FILE, TEST_WORD);
  EXPECT_EQ(err, ERR_NULL_PTR_REFERENCE);

  fscanner = fscanner_create();
  EXPECT_TRUE(fscanner != NULL);

  err = fscanner_count(fscanner, TEST_FILE, TEST_WORD);
  EXPECT_EQ(err, ERR_NIL);
  EXPECT_EQ(fscanner->count, TEST_COUNT);

  err = fscanner_count(fscanner, "invalid", TEST_WORD);
  EXPECT_NE(err, ERR_NIL);

  fscanner_free(fscanner);
}
