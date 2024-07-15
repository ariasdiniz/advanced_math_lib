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
Individuals *generate_individuals(
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
void destroy_individuals(Individuals *individuals);

/*
  This function get two high ranked (by fitness) individuals, take the 
  mean of ther weights and substitute in a low ranked individual,
  effectively "killing" the low ranked individual and creating a new one.
  This is done for the best (n_individuals * reproduction_rate) individuals.
*/
int reproduce(Individuals *individuals);

/*
  This function interates over every weight of every individual
  and slightly modify the weight if the randomized number is
  smaller or equal than the mutation chance.
*/
int mutate(Individuals *individuals);

/*
  This function calculates the fitness of every individual.
*/
int fit(Individuals *individuals, fitfunc func);

/*
----------------------------------------------------------------------------------
Kendall Correlation
*/

#include <stdio.h>
#include <stdlib.h>
#include <csvparser.h>

/*
  Calculates the Kendall Correlation between the two given arrays.
*/
double kcorr(double *data1, double *data2, size_t size);

/*
----------------------------------------------------------------------------------
Discrete Fourier Transform
*/
#include <complex.h>

/*
  Performs a Discrete Fourier Transform over the array data.
  This method modifies the original array. Returns 0 if successfull,
  Return -1 if not.
*/
int dft(double complex *data, size_t size, size_t n_threads);

#endif  // __ADVANCED_MATH_LIB