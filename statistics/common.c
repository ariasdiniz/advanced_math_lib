#include "../amath.h"
#include <math.h>
#include <stdlib.h>

double mean(double *data, size_t n_elements) {
  if (data == NULL || n_elements <= 0) return 0;
  double mean = 0;
  for (unsigned int i = 0; i < n_elements; i++) {
    mean += data[i];
  }
  return mean / n_elements;
}

double median(double *data, size_t n_elements) {
  if (data == NULL || n_elements <= 0) return 0;
  double median = 0;
  if (n_elements % 2 > 0) {
    return data[(n_elements - 1) / 2];
  } else {
    return (data[(n_elements - 1) / 2] + data[((n_elements - 1) / 2) + 1]) / 2;
  }
}

double stdev(double *data, size_t n_elements) {
  if (data == NULL || n_elements <= 0) return 0;
  double data_mean = mean(data, n_elements), square_sigma = 0;
  for (unsigned int i = 0; i < n_elements; i++) {
    square_sigma += pow(data[i] - data_mean, 2);
  }
  return sqrt(square_sigma / n_elements);
}
