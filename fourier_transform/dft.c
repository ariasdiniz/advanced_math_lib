/*
MIT License

Copyright (c) 2024 Aria Diniz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "../amath.h"
#include <pthread.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>

struct ArrayAndItem {
  double complex *array;
  size_t size;
  double complex *transform;
  unsigned int index;
};

static void *calc_xn(void *arg) {
  double complex *arr = ((struct ArrayAndItem *)arg)->array;
  double complex *transform = ((struct ArrayAndItem *)arg)->transform;
  unsigned int index = ((struct ArrayAndItem *)arg)->index;
  size_t size = ((struct ArrayAndItem *)arg)->size;
  double complex total = 0;

  for (unsigned int j = 0; j < size; j++) {
    total += arr[j] * cexp(-I * 2 * M_PI * index / size);
  }
  transform[index] = total;
  return NULL;
}

int dft(double complex *data, size_t size, size_t n_threads) {
  if (size <= 0 || n_threads <= 0) {
    return -1;
  }
  pthread_t threads[n_threads];
  double complex *transform = malloc(sizeof(double complex) * size);
  unsigned long count = 0;
  struct ArrayAndItem str[n_threads];
  unsigned int times = floor(size / n_threads);
  unsigned int remaining = size - (times * n_threads);
  
  for (unsigned int i = 0; i < n_threads; i++) {
    str[i].array = data;
    str[i].size = size;
    str[i].transform = transform;
  }

  for (unsigned int i = 0; i < times; i++) {
    for (unsigned int k = 0; k < n_threads; k++) {
      str[k].index = k + i * n_threads;
      pthread_create(&threads[k], NULL, calc_xn, &str[k]);
    }
    for (unsigned int k = 0; k < n_threads; k++) {
      pthread_join(threads[k], NULL);
    }
  }

  for (unsigned int i = 0; i < remaining; i++) {
    str[i].index = i + times * n_threads;
    pthread_create(&threads[i], NULL, calc_xn, &str[i]);
  }

  for (unsigned int i = 0; i < remaining; i++) {
    pthread_join(threads[i], NULL);
  }

  for (unsigned int i = 0; i < size; i++) {
    data[i] = transform[i];
  }
  free(transform);
  return 0;
}
