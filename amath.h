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

#ifndef __ADVANCED_MATH_LIB
#define __ADVANCED_MATH_LIB

/*
----------------------------------------------------------------------------------
Genetic Algorithm Session
*/

typedef struct Individual {
  float *weights;
  double fitness;
} Individual;

typedef struct Individuals {
  Individual **individual_array;
  int n_individuals;
  double mutation_prob, reproduction_rate, mutation_range;
  double number_weights, min, max;
} Individuals;

typedef void *fitfunc(Individuals *individuals);

/*
  Create a pointer to an array of individuals. The number of individuals
  is equal to the n_individuals provided. Don't forget to call
  destroy_individuals on this array after usage to free the
  used memory.
*/
Individuals *amath_generate_individuals(
  unsigned int n_individuals,
  double mutation_prob,
  double mutation_range,
  double reproduction_rate,
  double number_weights,
  double min,
  double max
);

/*
  Safely destroys an Individuals*
*/
void amath_destroy_individuals(Individuals *individuals);

/*
  This function get two high ranked (by fitness) individuals, take the mean of ther weights and substitute 
  in a low ranked individual, effectively "killing" the low ranked individual and creating a new one.
  This is done for the best (n_individuals * reproduction_rate) individuals.
*/
int amath_reproduce(Individuals *individuals);

/*
  This function interates over every weight of every individual and slightly modify the weight if the 
  randomized number is smaller or equal than the mutation chance.
*/
int amath_mutate(Individuals *individuals);

/*
  This function calculates the fitness of every individual.
*/
int amath_fit(Individuals *individuals, fitfunc func);

/*
----------------------------------------------------------------------------------
Kendall Correlation
*/

#include <stdio.h>
#include <stdlib.h>

/*
  Calculates the Kendall Correlation between the two given arrays.
  Returns -2 on error.
*/
double amath_kcorr(double *data1, double *data2, size_t size);

/*
----------------------------------------------------------------------------------
Discrete Fourier Transform
*/
#include <complex.h>

/*
  Performs a Discrete Fourier Transform over the array data. This method modifies the original array. 
  Use n_threads > 1 for multithreading. Returns 0 if successfull, Return -1 if not.
*/
int amath_dft(double complex *data, size_t size, size_t n_threads);

/*
  Performs an Inverse Fourier Transform over the array data. This method modifies the original array. 
  Use n_threads > 1 for multithreading. Returns 0 if successfull, Return -1 if not.
*/
int amath_inverse_dft(double complex *data, size_t size, size_t n_threads);

/*
----------------------------------------------------------------------------------
Mean
*/

/* Calculates the mean of the first n_elements of the values in the 1D array data.
   Return NAN if data is NULL or if n_elements <= 0.
*/
double amath_mean(double *data, size_t n_elements);

/*
----------------------------------------------------------------------------------
Median
*/

/* Calculates the median of the first n_elements of the values in the 1D array data.
   If sorted is 0, the data will be sorted using QuickSort.
   Return NAN if data is NULL or if n_elements <= 0.
*/
double amath_median(double *data, size_t n_elements, unsigned int sorted);

/*
----------------------------------------------------------------------------------
Standard Deviation
*/

/*
  Calculates the Standard Deviation of the first n_elements of the 1D array data.
  Use population = 1 for population stdev. 0 for sample stdev.
  Return NAN if data is NULL or if n_elements <= 0.
*/
double amath_stdev(double *data, unsigned int population, size_t n_elements);

/*
----------------------------------------------------------------------------------
Covariance
*/

/*
  Calculates the covariance between two datasets.
  Use population = 1 for population covariance (divide by n),
  or population = 0 for sample covariance (divide by n-1).
  Returns NAN on error (e.g. NULL pointers, mismatched sizes, or n_elements <= 1).
*/
double amath_covariance(
  double *data1,        /* pointer to first data array */
  double *data2,        /* pointer to second data array */
  unsigned int population, /* 1 = population covariance, 0 = sample covariance */
  size_t n_elements     /* number of elements in each array */
);
/*
----------------------------------------------------------------------------------
Pearson Correlation
*/

/*
  Calculates the Pearson correlation coefficient between two datasets.
  Returns a value in [−1, +1], or NAN on error
  (e.g. NULL pointers, mismatched sizes, or n_elements <= 1).
*/
double amath_pcorr(
  double *data1,      /* pointer to first data array */
  double *data2,      /* pointer to second data array */
  size_t n_elements   /* number of elements in each array */
);

/*
----------------------------------------------------------------------------------
Min
*/

/*
  Calculate the minimum value in a double array.
  Return NAN if n_elements is 0 or data is NULL.
*/
double amath_min(double* data, size_t n_elements);

/*
----------------------------------------------------------------------------------
Max
*/

/*
  Calculate the minimum value in a double array.
  Return NAN if n_elements is 0 or data is NULL.
*/
double amath_max(double* data, size_t n_elements);

/*
----------------------------------------------------------------------------------
Range
*/

/*
  Calculate the difference between min and max values of the array.
  Return NAN if n_elements is 0 or data is NULL.
*/
double amath_range(double* data, size_t n_elements);

/*
----------------------------------------------------------------------------------
Normalize
*/

/*
  Normalize the array values between 0 and 1. Does not normalize if
  Range or Min is NAN.
*/
void amath_normalize(double* data, size_t n_elements);

/*
----------------------------------------------------------------------------------
Normal Distribution
*/

/*
  Calculates the Normal Distribution of the first n_elements of the 1D array data,
  using n_threads from the system to concurrently calculate the values.
  Return a new 1D array with the distribution, or NULL on error. Don't forget to
  free the memory of the result after usage.
*/
double *amath_ndist(double *data, size_t n_elements, size_t n_threads);

/*
----------------------------------------------------------------------------------
Poisson Distribution
*/

/*
  Calculates the Poisson Distribution of the first n_elements of the 1D array data,
  using n_threads from the system to concurrently calculate the values.
  Return a new 1D array with the distribution, or NULL on error. Don't forget to
  free the memory of the result after usage.
*/
double *amath_pdist(int *data, double lambda, size_t n_elements, size_t n_threads);

#endif  // __ADVANCED_MATH_LIB
