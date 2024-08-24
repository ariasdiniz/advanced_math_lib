#include "../amath.h"
#include <stdio.h>
#include <math.h>

double *ndist(double *data, size_t n_elements) {
  if (data == NULL || n_elements == 0) return NULL;

  double *ndata = malloc(sizeof(double) * n_elements);
  if (ndata == NULL) return NULL;

  double avg = mean(data, n_elements);
  double deviation = stdev(data, n_elements);
  double normalization_factor = 1 / sqrt(2 * M_PI * pow(deviation, 2));
  double squared_dev = pow(deviation, 2);
  for (size_t i = 0; i < n_elements; i++) {
    ndata[i] = normalization_factor * exp(-pow((data[i] - avg), 2) / (2 * squared_dev));
  }
  return ndata;
}