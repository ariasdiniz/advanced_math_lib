#include "amath.h"
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <string.h>
#include <time.h>

#define HELP "AMath CLI\n"\
             "Aria Diniz - 2025\n\n"\
             "amath [CALC] - will calculate CALC for all values provided to STDIN\n\n"\
             "[CALC] -> mean, median, stdev, ndist, min, max, range, normalize, zscore, variance\n"\
             "This CLI does not handle complex numbers yet.\n"\

#define BUFFER_SIZE 120

static char TF[64];

static void store_data(size_t* count) {
  char buffer[BUFFER_SIZE];
  double temp_n;
  FILE* temp = fopen(TF, "w");
  if (!temp) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }
  while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
    *count += 1;
    temp_n = atof(buffer);
    fprintf(temp, "%lf\n", temp_n);
  }
  fclose(temp);
}

static double* allocate(size_t* count) {
  double* data = malloc(sizeof(double) * *count);
  if (data == NULL) {
    fprintf(stderr, "Error allocating memory for data processing.\n");
    return NULL;
  }
  return data;
}

static void read(FILE* temp, double* data, size_t* count) {
  char buffer[BUFFER_SIZE];
  for (size_t i = 0; i < *count; i++) {
    if (fgets(buffer, BUFFER_SIZE, temp) == NULL) {
      fprintf(stderr,  "Error parsing data.\n");
      break;
    }
    data[i] = atof(buffer);
  }
}

static int transform(size_t* count, char* func) {
  FILE* temp = fopen(TF, "r");
  if (!temp) {
    perror("fopen");
    return EXIT_FAILURE;
  }

  double* data = allocate(count);
  if (data == NULL) {
    fclose(temp);
    remove(TF);
    return EXIT_FAILURE;
  }

  read(temp, data, count);
  fclose(temp);
  remove(TF);

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

  struct timespec ts;
  if (clock_gettime(CLOCK_REALTIME, &ts) != 0) {
    perror("clock_gettime");
    return EXIT_FAILURE;
  }
  snprintf(TF, sizeof(TF),
           "amath_temp_%ld_%09ld.amath",
           (long)ts.tv_sec,
           ts.tv_nsec);

  size_t count = 0;
  store_data(&count);
  return transform(&count, argv[1]);
}

