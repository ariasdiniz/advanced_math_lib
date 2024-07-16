#include "../amath.h"
#include <pthread.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>

struct ArrayAndItem {
  double complex *array;
  size_t index_start;
  size_t index_end;
  size_t size;
  double complex *transform;
};

static void *calc_xn(void *arg) {
  struct ArrayAndItem *temp = ((struct ArrayAndItem *)arg);
  double complex *arr = temp->array;
  double complex *transform = temp->transform;
  size_t start = temp->index_start;
  size_t end = temp->index_end;
  size_t size = temp->size;
  double complex total;

  for (unsigned int k = start; k < end; k++) {
    total = 0;
    for (unsigned int j = 0; j < size; j++) {
      total += arr[j] * cexp(-I * 2 * M_PI * k * j / size);
    }
    transform[k] = total;
  }
  return NULL;
}

int dft(double complex *data, size_t size, size_t n_threads) {
  if (size <= 0 || n_threads <= 0) {
    return -1;
  }
  pthread_t threads[n_threads];
  double complex *transform = malloc(sizeof(double complex) * size);
  struct ArrayAndItem str[n_threads];
  unsigned int times = floor(size / n_threads);
  unsigned int remaining = size - (times * n_threads);
  
  for (unsigned int i = 0; i < n_threads; i++) {
    str[i].array = data;
    str[i].transform = transform;
    str[i].size = size;
  }

  for (unsigned int k = 0; k < n_threads; k++) {
    str[k].index_start = k * times;
    str[k].index_end = (k + 1) * times;
    if (k == n_threads - 1) str[k].index_end += remaining;
    pthread_create(&threads[k], NULL, calc_xn, &str[k]);
  }
  
  for (unsigned int k = 0; k < n_threads; k++) {
    pthread_join(threads[k], NULL);
  }

  for (unsigned int i = 0; i < size; i++) {
    data[i] = transform[i];
  }
  free(transform);
  return 0;
}
