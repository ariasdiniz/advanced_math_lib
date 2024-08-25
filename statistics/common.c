#include "../amath.h"
#include <math.h>
#include <stdlib.h>

static int sort_function(const void *a, const void *b) {
  double first = *(double *)a;
  double second = *(double *)b;

  if (first > second) return -1;
  if (second > first) return 1;
  return 0;
}

double amath_mean(double *data, size_t n_elements) {
  if (data == NULL || n_elements == 0) return 0;

  double mean = 0;
  for (size_t i = 0; i < n_elements; i++) {
    mean += data[i];
  }
  return mean / n_elements;
}

double amath_median(double *data, size_t n_elements, unsigned int sorted) {
  if (data == NULL || n_elements <= 0) return 0;

  double median = 0;
  if (!sorted) qsort(data, n_elements, sizeof(double), sort_function);

  if (n_elements % 2 > 0) {
    return data[(n_elements - 1) / 2];
  } else {
    return (data[(n_elements - 1) / 2] + data[((n_elements - 1) / 2) + 1]) / 2;
  }
}

double amath_stdev(double *data, size_t n_elements) {
  if (data == NULL || n_elements == 0) return 0;
  
  double data_mean = amath_mean(data, n_elements), square_sigma = 0;
  for (size_t i = 0; i < n_elements; i++) {
    square_sigma += pow(data[i] - data_mean, 2);
  }
  return sqrt(square_sigma / n_elements);
}

unsigned long long amath_factorial(unsigned int x) {
  if (x == 0) return 1;
  return amath_factorial(x - 1) * x;
}
