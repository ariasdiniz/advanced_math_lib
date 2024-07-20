#include "../amath.h"

typedef struct Data {
  double *x;
  double *y;
  size_t n;
} Data;

static double tau(Data *data) {
  double concordant = 0, discordant = 0;
  double dx, dy;
  for (size_t i = 0; i < data->n - 1; i++) {
    for (size_t j = i + 1; j < data->n; j++) {
      dx = data->x[i] - data->x[j];
      dy = data->y[i] - data->y[j];
      if (dx * dy > 0) {
        concordant++;
      } else if (dx * dy < 0) {
        discordant++;
      }
    }
  }
  size_t total_pairs = data->n * (data->n - 1) / 2;
  return (concordant - discordant) / (double)total_pairs;
}

double kcorr(double *data1, double *data2, size_t size) {
  Data *data = malloc(sizeof(Data));
  if (data == NULL) return -2.0;
  data->n = size;
  data->x = data1;
  data->y = data2;

  double t = tau(data);

  free(data);
  return t;
}