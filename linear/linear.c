#include "../amath.h"
#include <stdlib.h>

double amath_dprod(double* restrict vector1, double* restrict vector2, size_t n_elements) {
  double prod = 0;
  for (size_t i = 0; i < n_elements; i++) {
    prod += vector1[i] * vector2[i];
  }
  return prod;
}

double* amath_cprod(double* restrict vector1, double* restrict vector2, size_t n_elements) {
  if (vector1 == NULL || vector2 == NULL || n_elements != 3) return NULL;
  double* result = malloc(sizeof(double) * 3);
  if (result == NULL) return NULL;

  result[0] = vector1[1] * vector2[2] - vector1[2] * vector2[1];
  result[1] = vector1[2] * vector2[0] - vector1[0] * vector2[2];
  result[2] = vector1[0] * vector2[1] - vector1[1] * vector2[0];

  return result;
}

