#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../amath.h"

static float random_number(double min, double max) {
  float scale = rand() / (float)RAND_MAX;
  return min + scale * (max - min);
}

static void destroy_individual_array(Individual **individuals, unsigned int n_individuals) {
  for (int i = 0; i < n_individuals; i++) {
    free(individuals[i]->weights);
    free(individuals[i]);
  }
  free(individuals);
}

static int compare_individuals(const void* a, const void* b) {
    const Individual *arg1 = *(const Individual **)a;
    const Individual *arg2 = *(const Individual **)b;

    if (arg1->fitness > arg2->fitness) return -1;
    if (arg1->fitness < arg2->fitness) return 1;
    return 0;
}

static void reproduction(Individual *ind1, Individual *ind2, Individual *result, unsigned int n_weights) {
  for (int i = 0; i < n_weights; i++) {
    result->weights[i] = (ind1->weights[i] + ind2->weights[i]) / 2.0;
  }
}

static float mutation_prob() {
  return rand() / (float)RAND_MAX;
}

static float mutation(double mutation_range) {
  float scale = mutation_prob() * mutation_range;
  return (-mutation_range / 2.0) + scale;
}

Individuals *generate_individuals(
  unsigned int n_individuals,
  double mutation_prob,
  double mutation_range,
  double reproduction_rate,
  double number_weights,
  double min,
  double max
) {
  if (mutation_prob > 1 || mutation_prob < 0) {
    fprintf(stderr, "mutation_prob should be a number between 0 and 1.\n");
    exit(EXIT_FAILURE);
  }
  if (reproduction_rate > 1 || reproduction_rate < 0) {
    fprintf(stderr, "reproduction_rate should be a number between 0 and 1.\n");
    exit(EXIT_FAILURE);
  }
  Individual **individual_array = malloc(sizeof(Individual *) * n_individuals);
  for (int i = 0; i < n_individuals; i ++) {
    individual_array[i] = malloc(sizeof(Individual));
    if (individual_array[i] == NULL) {
      for (int j = 0; j < i; j++) {
        free(individual_array[j]);
      }
      free(individual_array);
      return NULL;
    }
  }
  Individuals *individuals = malloc(sizeof(Individuals));
  if (individuals == NULL) {
    destroy_individual_array(individual_array, n_individuals);
    return NULL;
  }
  individuals->individual_array = individual_array;
  individuals->n_individuals = n_individuals;
  individuals->max = max;
  individuals->min = min;
  individuals->mutation_prob = mutation_prob;
  individuals->mutation_range = mutation_range;
  individuals->number_weights = number_weights;
  individuals->reproduction_rate = reproduction_rate;
  for (int i = 0; i < n_individuals; i++) {
    individual_array[i]->weights = malloc(sizeof(float) * number_weights);
    for (int j = 0; j < number_weights; j++) {
      individual_array[i]->weights[j] = random_number(min, max);
    }
    individual_array[i]->fitness = 0.0;
  }
  return individuals;
}

void destroy_individuals(Individuals *individuals) {
  destroy_individual_array(individuals->individual_array, individuals->n_individuals);
  free(individuals);
}

int reproduce(Individuals *individuals) {
  if (individuals == NULL) {
    return -1;
  }
  Individual **individual_array = individuals->individual_array;
  unsigned int array_size = individuals->n_individuals;
  unsigned int individuals_to_reproduce = floor(array_size * individuals->reproduction_rate);
  qsort(individual_array, array_size, sizeof(Individual*), compare_individuals);
  for (int i = 0; i < individuals_to_reproduce; i ++) {
    reproduction(individual_array[i*2], individual_array[(i*2)+1], individual_array[array_size-1-i], individuals->number_weights);
  }
  return 0;
}

int mutate(Individuals *individuals) {
  if (individuals == NULL || individuals->number_weights < 1) {
    return -1;
  }
  for (unsigned int i = 0; i < individuals->n_individuals; i++) {
    for (unsigned int j = 0; j < individuals->number_weights; j++) {
      if (mutation_prob() <= individuals->mutation_prob) {
        individuals->individual_array[i]->weights[j] += mutation(individuals->mutation_range);
      }
    }
  }
  return 0;
}

int fit(Individuals *individuals, fitfunc func) {
  if (individuals == NULL) {
    return -1;
  }
  func(individuals);
  return 0;
}
