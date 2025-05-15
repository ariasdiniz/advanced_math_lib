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
* **Covariance**: Measure how two datasets vary together. Returns `NAN` on error and sets `errno`.
* **Pearson Correlation**: Calculate the Pearson correlation coefficient (r ∈ \[−1, +1]). Returns `NAN` on error and sets `errno`.
* **Kendall's Tau**: Calculate the Kendall rank correlation coefficient between two datasets.

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

To use libamath, clone this repository and include the source files in your project. The library is licensed under the MIT license.

```bash
git clone git@github.com:ariasdiniz/advanced_math_lib.git
```

To build locally (requires GCC and Make):

```shell
make
```

This produces:

* `libamath.so` (shared library)
* `amath` (CLI executable)

Link against with `-lamath -lm`.

Alternatively, run `sudo ./install.sh` to install system-wide.

## Usage

Here’s an example showing covariance and correlation:

```c
#include <math.h>
#include <stdio.h>
#include "amath.h"

int main() {
    double a[] = {1.0, 2.0, 3.0};
    double b[] = {2.0, 4.0, 6.0};

    errno = 0;
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

* `mean`, `median`, `stdev`, `ndist`

Future CLI will include `covariance` and `pcorr`.

## Contributing

Contributions are welcome! Fork the repo and submit a pull request for new features or bug fixes. For issues, please open a GitHub Issue. Feedback and suggestions are always appreciated.

