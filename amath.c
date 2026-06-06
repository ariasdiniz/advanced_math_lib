#include "amath.h"
#include <complex.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HELP "AMath CLI\n"\
             "Aria Diniz - 2025\n\n"\
             "amath [CALC] - will calculate CALC for all values provided to STDIN\n\n"\
             "[CALC] -> mean, median, stdev, ndist, min, max, range, normalize, zscore, variance\n"\
             "This CLI does not handle complex numbers yet.\n"\

#define BUFFER_SIZE 120
#define DATA_CAPACITY 10000

typedef void (*math_handler_t)(double* data, size_t count);

typedef struct {
  const char* name;
  math_handler_t handler;
} Command;

static void handle_mean(double* data, size_t count) {
  printf("%lf\n", amath_mean(data, count));
}

static void handle_median(double* data, size_t count) {
  printf("%lf\n", amath_median(data, count, 0));
}

static void handle_stdev(double* data, size_t count) {
  printf("%lf\n", amath_stdev(data, 1, count));
}

static void handle_ndist(double* data, size_t count) {
  double* dist = amath_ndist(data, count, 4);
  for (size_t i = 0; i < count; i++) printf("%lf\n", dist[i]);
  free(dist);
}

static void handle_min(double* data, size_t count) {
  printf("%lf\n", amath_min(data, count));
}

static void handle_max(double* data, size_t count) {
  printf("%lf\n", amath_max(data, count));
}

static void handle_range(double* data, size_t count) {
  printf("%lf\n", amath_range(data, count));
}

static void handle_normalize(double* data, size_t count) {
  amath_normalize(data, count);
  for (size_t i = 0; i < count; i++) printf("%lf\n", data[i]);
}

static void handle_zscore(double* data, size_t count) {
  double* zscore = amath_zscore(data, count);
  for (size_t i = 0; i < count; i++) printf("%lf\n", zscore[i]);
  free(zscore);
}

static void handle_variance(double* data, size_t count) {
  printf("%lf\n", amath_variance(data, count));
}

static const Command commands[] = {
  {"max", handle_max},
  {"mean", handle_mean},
  {"median", handle_median},
  {"min", handle_min},
  {"ndist", handle_ndist},
  {"normalize", handle_normalize},
  {"range", handle_range},
  {"stdev", handle_stdev},
  {"variance", handle_variance},
  {"zscore", handle_zscore}
};

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
  size_t num_commands = sizeof(commands) / sizeof(commands[0]);
  
  for (size_t i = 0; i < num_commands; i++) {
    if (strcmp(func, commands[i].name) == 0) {
      commands[i].handler(data, *count);
      free(data);
      return EXIT_SUCCESS;
    }
  }
  
  fprintf(stderr, "Unknown option: %s\n", func);
  free(data);
  return EXIT_FAILURE;
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
