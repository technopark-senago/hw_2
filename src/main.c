// Copyright 2021 Oganes Mirzoyan mirzoyan.oganes@mail.ru

#include <stdio.h>
#include <time.h>

#include "constants.h"
#include "dscanner.h"

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Invalid input: [directory path] and [word] are expected\n");
    return ERR_INVALID_INPUT;
  }

  dscanner_t* dscanner = dscanner_create();
  if (dscanner == NULL) {
    return ERR_MEMORY_ALLOCATION;
  }

  printf("Scanning files in [%s] for word [%s]...\n", argv[1], argv[2]);

  struct timespec start = {0, 0};
  struct timespec stop = {0, 0};
  if (clock_gettime(CLOCK_REALTIME, &start) == -1) {
    dscanner_free(dscanner);
    return ERR_CLOCK_GETTIME;
  }

  err_t err = dscanner_scan(dscanner, argv[1], argv[2]);
  if (err != ERR_NIL) {
    dscanner_free(dscanner);
    return err;
  }

  if (clock_gettime(CLOCK_REALTIME, &stop) == -1) {
    dscanner_free(dscanner);
    return ERR_CLOCK_GETTIME;
  }

  dscanner_print_results(dscanner);

  double accum = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) / 1e9;
  printf("Scanning took [%f] seconds\n", accum);

  dscanner_free(dscanner);

  return 0;
}
