// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#include <gtest/gtest.h>

#include <string>

extern "C" {
#include "dscanner.h"
}

constexpr char TEST_DIR[] = "static/data";
constexpr char TEST_WORD[] = "word";

dscanner_t* test_dscanner_create() {
  dscanner_t* dscanner = dscanner_create();
  EXPECT_TRUE(dscanner != NULL);
  return dscanner;
}

void test_dscanner_scan(dscanner_t* dscanner) {
  err_t err = dscanner_scan(dscanner, "invalid", TEST_WORD);
  EXPECT_TRUE(err != ERR_NIL);

  err = dscanner_scan(dscanner, TEST_DIR, TEST_WORD);
  EXPECT_EQ(err, ERR_NIL);
}

TEST(dscanner_t, dscanner_create) {
  dscanner_t* dscanner = NULL;
  dscanner_free(dscanner);

  dscanner = dscanner_create();
  EXPECT_TRUE(dscanner != NULL);

  dscanner_free(dscanner);
}

TEST(dscanner_t, dscanner_scan) {
  dscanner_t* dscanner = test_dscanner_create();
  test_dscanner_scan(dscanner);
  dscanner_free(dscanner);
}
