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

#define transform_element(x, k, j, size) ((x) * cexp(-I * 2 * M_PI * (k) * (j) / (size)))

static void *calc_xn(void *arg) {
  struct ArrayAndItem *temp = ((struct ArrayAndItem *)arg);
  double complex *arr = temp->array;
  double complex *transform = temp->transform;
  size_t start = temp->index_start;
  size_t end = temp->index_end;
  size_t size = temp->size;

  for (size_t k = start; k < end; k++) {
    double complex total = 0;
    for (size_t j = 0; j < size; j++) {
      total += transform_element(arr[j], k, j, size);
    }
    transform[k] = total;
  }
  return NULL;
}

int amath_dft(double complex *data, size_t size, size_t n_threads) {
  if (data == NULL || size == 0 || n_threads == 0) {
    return -1;
  }

  double complex *transform;
  if (posix_memalign((void **)&transform, 64, sizeof(double complex) * size) != 0) {
    return -1;
  }

  struct ArrayAndItem *str;
  if (posix_memalign((void **)&str, 64, sizeof(struct ArrayAndItem) * n_threads) != 0) {
    free(transform);
    return -1;
  }

  pthread_t threads[n_threads];
  size_t step = size / n_threads;
  size_t remaining = size % n_threads;

  for (size_t i = 0; i < n_threads; i++) {
    str[i].array = data;
    str[i].transform = transform;
    str[i].size = size;
    str[i].index_start = i * step;
    str[i].index_end = (i + 1) * step + (i == n_threads - 1 ? remaining : 0);

    if (pthread_create(&threads[i], NULL, calc_xn, &str[i]) != 0) {
      free(transform);
      free(str);
      return -1;
    }
  }

  for (size_t i = 0; i < n_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  for (size_t i = 0; i < size; i++) {
    data[i] = transform[i];
  }

  free(transform);
  free(str);
  return 0;
}

#define inverse_element(x, n, k, size) ((x) * cexp(2 * M_PI * I * (n) * (k) / (size)))

static void *calc_inverse_xn(void *data) {
  struct ArrayAndItem *d = (struct ArrayAndItem *)data;
  size_t index_start = d->index_start, index_end = d->index_end, size = d->size;
  double complex *arr = d->array;
  double complex *inverse_transform = d->transform;

  for (size_t i = index_start; i < index_end; i++) {
    double complex total = 0;
    for (size_t k = 0; k < size; k++) {
      total += inverse_element(arr[k], i, k, size);
    }
    inverse_transform[i] = total / size;
  }
  return NULL;
}

int amath_inverse_dft(double complex *data, size_t size, size_t n_threads) {
  if (data == NULL || size == 0 || n_threads == 0) return -1;

  double complex *inverse_transform;
  if (posix_memalign((void **)&inverse_transform, 64, sizeof(double complex) * size) != 0) {
    return -1;
  }

  struct ArrayAndItem *transform;
  if (posix_memalign((void **)&transform, 64, sizeof(struct ArrayAndItem) * n_threads) != 0) {
    free(inverse_transform);
    return -1;
  }

  pthread_t threads[n_threads];
  size_t step = size / n_threads;

  for (size_t i = 0; i < n_threads; i++) {
    transform[i].array = data;
    transform[i].index_start = step * i;
    transform[i].index_end = (i < n_threads - 1) ? step * (i + 1) : size;
    transform[i].size = size;
    transform[i].transform = inverse_transform;

    if (pthread_create(&threads[i], NULL, calc_inverse_xn, &transform[i]) != 0) {
      free(inverse_transform);
      free(transform);
      return -1;
    }
  }

  for (size_t i = 0; i < n_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  for (size_t i = 0; i < size; i++) {
    data[i] = inverse_transform[i];
  }

  free(inverse_transform);
  free(transform);
  return 0;
}
