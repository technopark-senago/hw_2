// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#include <stdio.h>
#include <time.h>

#include "constants.h"
#include "dscanner.h"

err_t run(const char* dir_path, const char* word, double* elapsed_time) {
  dscanner_t* dscanner = dscanner_create();
  if (dscanner == NULL) {
    return ERR_MEMORY_ALLOCATION;
  }

  struct timespec start = {0, 0};
  struct timespec stop = {0, 0};
  if (clock_gettime(CLOCK_REALTIME, &start) == -1) {
    dscanner_free(dscanner);
    return ERR_CLOCK_GETTIME;
  }

  err_t err = dscanner_scan(dscanner, dir_path, word);
  if (err != ERR_NIL) {
    dscanner_free(dscanner);
    return err;
  }

  if (clock_gettime(CLOCK_REALTIME, &stop) == -1) {
    dscanner_free(dscanner);
    return ERR_CLOCK_GETTIME;
  }

  dscanner_free(dscanner);
  *elapsed_time = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;

  return ERR_NIL;
}

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Invalid input: [directory path] and [word] are expected\n");
    return ERR_INVALID_INPUT;
  }

  size_t amount_of_scans = 3;
  double total_elapsed_time = 0;

  for (size_t i = 0; i < amount_of_scans; ++i) {
    double elapsed_time = 0;
    err_t err = run(argv[1], argv[2], &elapsed_time);
    if (err != ERR_NIL) {
      return err;
    }
    total_elapsed_time += elapsed_time;
  }

  double average_elapsed_time = total_elapsed_time / amount_of_scans;
  printf("Scanning [%s] for word [%s] took [%f] seconds in average\n", argv[1], argv[2], average_elapsed_time);

  return 0;
}
