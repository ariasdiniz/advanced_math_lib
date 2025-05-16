## libamath - Advanced Math Library

**libamath** is a C library that provides advanced mathematical functions, with a focus on algorithms for statistical analysis and genetic algorithms. The library is optimized for performance, with multithreaded implementations for various distributions and transforms, making it suitable for computationally intensive tasks.

## Features

### Genetic Algorithms

* **Individuals generation**: Create a population of individuals, each with a set of weights. Includes mutation and reproduction mechanisms.
* **Reproduction**: Replaces low-fitness individuals by reproducing the best-performing individuals, using the mean of their weights.
* **Mutation**: Randomly alters the weights of individuals based on a mutation probability.
* **Fitness evaluation**: A user-defined function to evaluate the fitness of each individual in the population.

### Statistical Functions

* **Mean**: Calculate the mean of a dataset. Returns `NAN` on error (NULL pointer or zero length).
* **Median**: Compute the median of a dataset, with an option to pre-sort the data. Returns `NAN` on error.
* **Standard Deviation**: Compute the population or sample standard deviation. Returns `NAN` on error.
* **Covariance**: Measure how two datasets vary together. Returns `NAN` on error`.
* **Pearson Correlation**: Calculate the Pearson correlation coefficient (r ∈ \[−1, +1]). Returns `NAN` on error.
* **Kendall's Tau**: Calculate the Kendall rank correlation coefficient between two datasets.
* **Min**: Return the smallest value in the array. Returns `NAN` on error.
* **Max**: Return the largest value in the array. Returns `NAN` on error.
* **Range**: Calculate the range of a dataset (max - min). Returns `NAN` on error.
* **Normalize**: Normalize a dataset to a specified range, using min-max normalization. Does not normalize if Range is zero or Range or Min are `NAN`.

### Discrete Fourier Transform (DFT)

* **DFT**: Perform a Discrete Fourier Transform on a dataset, with multithreading support for faster execution.
* **Inverse DFT**: Perform an inverse DFT to revert transformed data back to the time domain.

### Probability Distributions

* **Normal Distribution**: Calculate the normal distribution values of a dataset, optimized with multithreading.
* **Poisson Distribution**: Compute the Poisson distribution of a discrete dataset, also supporting multithreading.

## Future Work

Planned enhancements:

* **Variance Calculation**: Complement standard deviation with a direct variance function.
* **Linear Regression**: Model linear relationships between variables.
* **Binomial Distribution**: Add support for binomial probabilities.
* **Gamma Distribution**: Provide gamma distribution computations.
* **CLI Extensions**: Expose `covariance`, `pcorr`, and other new functions in the `amath` command-line tool.

## Installation

To use libamath, simply clone this repository and include the source files in your project. The library is licensed under the MIT license, so you are free to use, modify, and distribute it in your own projects. When using `libamath` don't forget to link `-lamath -lm` during compilation.

```bash
git clone git@github.com:ariasdiniz/advanced_math_lib.git
```

If you want to install it in your UNIX system you can use the `install.sh` script. It needs superuser access and GCC compiler instaled.

If you have Make installed, you can also build with

```shell
make
```

This way you will build the lib's Shared Object and also an executable called `amath`. This executable is a simple command line tool
to run functions like `stdev`, `mean`, `ndist` and `median` in a stream of data read from STDIN.

## Usage

Here’s a simple example of how to use the genetic algorithm functionality in libamath:

```c
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <amath.h>

/*
  This code aims to demonstrate the functioning of the genetic algorithm.

  For this, we have the following problem:

  We have a basket that can carry 8 kg, and we have a set of 4 products.
  Their weights are 3 kg, 4 kg, 5kg and 8 kg, represented by the algorithm's
  weights 0, 1, 2 and 3 respectively.

  The problem is to determine the number of unique products
  this basket can carry. The response is 2 items, one that wights 3 kg
  and another that weights 5 kg. Or it could be only the item with 8 kg.

  For this problem, the best fitted individuals of the algorithm
  have to have, after training, weights like:

  0 => between 0.2 and 1
  1 => approximately 0
  2 => between 0.2 and 1
  3 => approximately 0

  OR

  0 => approximately 0
  1 => approximately 0
  2 => approximately 0
  3 => between 0.2 and 1

  For better results, increase the number of individuals and the
  number of iterations, at the cost of greater computing time.
*/

void *fun(Individuals *individuals) {
  double result = 0;
  int array_size = individuals->n_individuals;
  Individual **individual = individuals->individual_array;
  for (int i = 0; i < array_size; i++) {
    if (individual[i]->weights[0] >= 0.01) result += 3;
    if (individual[i]->weights[1] >= 0.01) result += 4;
    if (individual[i]->weights[2] >= 0.01) result += 5;
    if (individual[i]->weights[3] >= 0.01) result += 8;
    if (result > 8 || result <= 0 ) {
      individual[i]->fitness = 0.0;
      result = 0;
      continue;
    }
    individual[i]->fitness = result;
    result = 0;
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  srand(time(NULL));
  Individuals *individuals = generate_individuals(100000, 0.05, 0.0001, 0.25, 4, 0.0, 1.0);
  for (int i = 0; i < 1000; i++) {
    fit(individuals, fun);
    mutate(individuals);
    reproduce(individuals);
  }
  printf("Individual 1, weight 0 : %f\n", individuals->individual_array[0]->weights[0]);
  printf("Individual 1, weight 1 : %f\n", individuals->individual_array[0]->weights[1]);
  printf("Individual 1, weight 2 : %f\n", individuals->individual_array[0]->weights[2]);
  printf("Individual 1, weight 3 : %f\n", individuals->individual_array[0]->weights[3]);
  destroy_individuals(individuals);
  return 0;
}
```

## Another Usage

Here’s an example showing covariance and correlation:

```c
#include <math.h>
#include <stdio.h>
#include "amath.h"

int main() {
    double a[] = {1.0, 2.0, 3.0};
    double b[] = {2.0, 4.0, 6.0};

    double cov = amath_covariance(a, b, 1, 3);
    if (isnan(cov)) {
        return 1;
    }
    printf("Covariance: %f\n", cov);

    double r = amath_pcorr(a, b, 3);
    if (isnan(r)) {
        return 1;
    }
    printf("Pearson r: %f\n", r);

    return 0;
}
```

## CLI Usage

After building, use the `amath` tool to process data streams:

```shell
cat data.txt | amath median
```

Supported commands:

* `mean`, `median`, `stdev`, `ndist`, `min`, `max`, `range`, `normalize`

Future CLI will include `covariance` and `pcorr`.

## Contributing

Contributions are welcome! Fork the repo and submit a pull request for new features or bug fixes. For issues, please open a GitHub Issue. Feedback and suggestions are always appreciated.

