#include "../amath.h"
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

struct calc_segment {
  size_t interval_a, interval_b;
  double *data, *normalized_data;
  double normalization_factor, avg, squared_dev;
};

#define calculate_ndist_point(x, norm, avg, sqdev) ((norm) * exp(-pow(((x) - (avg)), 2) / (2 * (sqdev))))

void *calculation_segment(void *data) {
  struct calc_segment *segment = (struct calc_segment *)data;
  size_t lim_a = segment->interval_a, lim_b = segment->interval_b;
  double *d = segment->data;
  double *normalized_data = segment->normalized_data;
  double norm_factor = segment->normalization_factor, avg = segment->avg, squared_dev = segment->squared_dev;

  for (size_t i = lim_a; i < lim_b; i++) {
    normalized_data[i] = calculate_ndist_point(d[i], norm_factor, avg, squared_dev);
  }
  return NULL;
}

double *amath_ndist(double *data, size_t n_elements, size_t n_threads) {
  if (data == NULL || n_elements == 0 || n_threads == 0) return NULL;

  double *ndata;
  if (posix_memalign((void **)&ndata, 64, sizeof(double) * n_elements) != 0) {
    return NULL;
  }

  struct calc_segment *temp_data;
  if (posix_memalign((void **)&temp_data, 64, sizeof(struct calc_segment) * n_threads) != 0) {
    free(ndata);
    return NULL;
  }

  double avg = amath_mean(data, n_elements);
  double deviation = amath_stdev(data, 1, n_elements);
  double normalization_factor = 1 / sqrt(2 * M_PI * pow(deviation, 2));
  double squared_dev = pow(deviation, 2);
  
  size_t num_threads = n_threads <= n_elements ? n_threads : n_elements;

  pthread_t threads[num_threads];
  size_t step = n_elements / num_threads;

  for (size_t i = 0; i < num_threads; i++) {
    temp_data[i].avg = avg;
    temp_data[i].data = data;
    temp_data[i].normalization_factor = normalization_factor;
    temp_data[i].normalized_data = ndata;
    temp_data[i].squared_dev = squared_dev;
    temp_data[i].interval_a = step * i;

    if (i < num_threads - 1) {
      temp_data[i].interval_b = step * (i + 1);
    } else {
      temp_data[i].interval_b = n_elements;
    }

    if (pthread_create(&threads[i], NULL, calculation_segment, &temp_data[i]) != 0) {
      free(ndata);
      free(temp_data);
      return NULL;
    }
  }

  for (size_t i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  free(temp_data);
  return ndata;
}

struct pdist_segment {
  size_t interval_a, interval_b;
  double lambda;
  int *data;
  double *pdist;
};

#define calculate_pdist_point(k, lambda) (pow((lambda), (k)) * exp(-(lambda)) / tgamma(k + 1))

void *calculate_pdist_segment(void *data) {
  struct pdist_segment *segment = (struct pdist_segment *)data;
  size_t interval_a = segment->interval_a, interval_b = segment->interval_b;
  double lambda = segment->lambda;
  int *d = segment->data;

  for (size_t i = interval_a; i < interval_b; i++) {
    segment->pdist[i] = calculate_pdist_point(d[i], lambda);
  }
  return NULL;
}

double *amath_pdist(int *data, double lambda, size_t n_elements, size_t n_threads) {
  if (data == NULL || n_elements == 0 || n_threads == 0) return NULL;

  double *pdist;
  if (posix_memalign((void **)&pdist, 64, sizeof(double) * n_elements) != 0) {
    return NULL;
  }

  size_t num_threads = n_threads <= n_elements ? n_threads : n_elements;

  pthread_t threads[num_threads];
  struct pdist_segment *segments;
  if (posix_memalign((void **)&segments, 64, sizeof(struct pdist_segment) * num_threads) != 0) {
    free(pdist);
    return NULL;
  }

  size_t step = n_elements / num_threads;

  for (size_t i = 0; i < num_threads; i++) {
    segments[i].data = data;
    segments[i].lambda = lambda;
    segments[i].pdist = pdist;
    segments[i].interval_a = step * i;

    if (i < num_threads - 1) {
      segments[i].interval_b = step * (i + 1);
    } else {
      segments[i].interval_b = n_elements;
    }

    if (pthread_create(&threads[i], NULL, calculate_pdist_segment, &segments[i]) != 0) {
      free(pdist);
      free(segments);
      return NULL;
    }
  }

  for (size_t i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  free(segments);
  return pdist;
}
