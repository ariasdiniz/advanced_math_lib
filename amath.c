#include "amath.h"
#include <complex.h>
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <string.h>

#define HELP "AMath CLI\n"\
             "Aria Diniz - 2025\n\n"\
             "amath [CALC] - will calculate CALC for all values provided to STDIN\n\n"\
             "[CALC] -> mean, median, stdev, ndist, min, max, range, normalize, zscore, variance\n"\
             "This CLI does not handle complex numbers yet.\n"\

#define BUFFER_SIZE 120
#define DATA_CAPACITY 10000

static double* store_data(size_t* count) {
  char buffer[BUFFER_SIZE];
  int realloc_count = 1;
  double* data = malloc(sizeof(double) * DATA_CAPACITY);
  if (!data) {
    perror("Error allocating memory to parse data");
    exit(EXIT_FAILURE);
  }
  while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
    data[*count] = atof(buffer);
    *count += 1;
    if (*count == DATA_CAPACITY * realloc_count) {
      realloc_count *= 2;
      double* check = realloc(data, sizeof(double) * DATA_CAPACITY * realloc_count);
      if (!check) {
        free(data);
        perror("Error allocating memory to parse data");
        exit(EXIT_FAILURE);
      }
      data = check;
    }
  }
  return data;
}

static int transform(double* data, size_t* count, char* func) {

  if (strcmp(func, "mean") == 0) {
    printf("%lf\n", amath_mean(data, *count));
  } else if (strcmp(func, "median") == 0) {
    printf("%lf\n", amath_median(data, *count, 0));
  } else if (strcmp(func, "stdev") == 0) {
    printf("%lf\n", amath_stdev(data, 1, *count));
  } else if (strcmp(func, "ndist") == 0) {
    double* dist = amath_ndist(data, *count, 4);
    for (size_t i = 0; i < *count; i++) printf("%lf\n", dist[i]);
    free(dist);
  } else if (strcmp(func, "min") == 0) {
    printf("%lf\n", amath_min(data, *count));
  } else if (strcmp(func, "max") == 0) {
    printf("%lf\n", amath_max(data, *count));
  } else if (strcmp(func, "range") == 0) {
    printf("%lf\n", amath_range(data, *count));
  } else if (strcmp(func, "normalize") == 0) {
    amath_normalize(data, *count);
    for (size_t i = 0; i < *count; i++) printf("%lf\n", data[i]);
  } else if (strcmp(func, "zscore") == 0) {
    double* zscore = amath_zscore(data, *count);
    for (size_t i = 0; i < *count; i++) printf("%lf\n", zscore[i]);
    free(zscore);
  } else if (strcmp(func, "variance") == 0 ) {
    printf("%lf\n", amath_variance(data, *count));
  } else {
    fprintf(stderr, "Unknown option: %s\n", func);
    free(data);
    return EXIT_FAILURE;
  }

  free(data);
  return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "Amath needs at least one positional argument. Try --help\n");
    return EXIT_FAILURE;
  }
  if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
    printf(HELP);
    return EXIT_SUCCESS;
  }

  size_t count = 0;
  double* data = store_data(&count);
  return transform(data, &count, argv[1]);
}

