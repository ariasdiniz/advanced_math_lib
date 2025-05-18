#include "../amath.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static int sort_function(const void *a, const void *b) {
  double first = *(double *)a;
  double second = *(double *)b;

  if (first > second) return -1;
  if (second > first) return 1;
  return 0;
}

double amath_mean(double *data, size_t n_elements) {
  if (data == NULL || n_elements == 0) return NAN;

  double mean = 0;
  for (size_t i = 0; i < n_elements; i++) {
    mean += data[i];
  }
  return mean / n_elements;
}

double amath_median(double *data, size_t n_elements, unsigned int sorted) {
  if (data == NULL || n_elements <= 0) return NAN;

  if (!sorted) qsort(data, n_elements, sizeof(double), sort_function);

  if (n_elements % 2 > 0) {
    return data[(n_elements - 1) / 2];
  } else {
    return (data[(n_elements - 1) / 2] + data[((n_elements - 1) / 2) + 1]) / 2;
  }
}

double amath_stdev(double *data, unsigned int population, size_t n_elements) {
  if (data == NULL || n_elements == 0) return NAN;
  int bessel_correction = population ? 0 : 1;
  
  double data_mean = amath_mean(data, n_elements), square_sigma = 0;
  for (size_t i = 0; i < n_elements; i++) {
    square_sigma += pow(data[i] - data_mean, 2);
  }
  return sqrt(square_sigma / (n_elements - bessel_correction));
}

double amath_min(double* data, size_t n_elements) {
  if (data == NULL || n_elements == 0) return NAN;
  double min = data[0];
  for (size_t i = 1; i < n_elements; i++) {
    if (min > data[i]) min = data[i];
  }
  return min;
}

double amath_max(double* data, size_t n_elements) {
  if (data == NULL || n_elements == 0) return NAN;
  double max = data[0];
  for (size_t i = 1; i < n_elements; i++) {
    if (max < data[i]) max = data[i];
  }
  return max;
}

double amath_range(double* data, size_t n_elements) {
  if (data == NULL || n_elements < 1) return NAN;
  return amath_max(data, n_elements) - amath_min(data, n_elements);
}

void amath_normalize(double* data, size_t n_elements) {
  if (data == NULL || n_elements == 0) return;
  double min = amath_min(data, n_elements);
  double range = amath_range(data, n_elements);
  if (isnan(range) || isnan(min) || range == 0) return;

  for (size_t i = 0; i < n_elements; i++) {
    data[i] = (data[i] - min) / range;
  }
}

double amath_covariance(double* data, double* other, unsigned int population, size_t n_elements) {
  if (data == NULL || other == NULL || n_elements < 1) return NAN;
  double xmean = amath_mean(data, n_elements);
  double ymean = amath_mean(other, n_elements);

  if (isnan(xmean) || isnan(ymean)) return NAN;

  double sum = 0;
  unsigned int bessel_correction = population ? 0 : 1;

  for (size_t i = 0; i < n_elements; i++) {
    sum += (data[i] - xmean) * (other[i] - ymean);
  }

  return sum / (n_elements - bessel_correction);
}

double amath_pcorr(double* data, double* other, size_t n_elements) {
  if (data == NULL || other == NULL || n_elements < 1) return NAN;
  double covariance = amath_covariance(data, other, 1, n_elements);
  double xstdev = amath_stdev(data, 1, n_elements);
  double ystdev = amath_stdev(other, 1, n_elements);

  if (isnan(xstdev) || isnan(ystdev) || isnan(covariance)) return NAN;;
  if (xstdev == 0 || ystdev == 0) return NAN;

  return covariance / (xstdev * ystdev);
}

double* amath_zscore(double* data, size_t n_elements) {
  if (data == NULL || n_elements < 1) return NULL;
  
  double stdev = amath_stdev(data, 1, n_elements);
  if (isnan(stdev)) return NULL;
  
  double mean = amath_mean(data, n_elements);
  if (isnan(mean)) return NULL;

  double* zscore = malloc(sizeof(double) * n_elements);
  if (zscore == NULL) return NULL;
  
  for (size_t i = 0; i < n_elements; i++) {
    zscore[i] = (data[i] - mean) / stdev;
  }

  return zscore;
}

double amath_variance(double *data, size_t n_elements) {
  if (data == NULL || n_elements < 1) return NAN;
  return amath_covariance(data, data, 1, n_elements);
}

