#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void sieve_of_eratosthenes(int n, const char *filename) {
    // Initialize an array to store prime numbers
    char *is_prime = (char *)malloc((n + 1) * sizeof(char));
    if (is_prime == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    #pragma omp parallel for
    for (int i = 2; i <= n; i++) {
        is_prime[i] = 1;
    }

    // Apply the sieve algorithm
    #pragma omp parallel
    for (int p = 2; p * p <= n; p++) {
        #pragma omp for
        for (int i = p * p; i <= n; i += p) {
            if (i > 0 && i <= n) {
                is_prime[i] = 0;
            }
        }
    }

    // Write prime numbers to a file
    FILE *outfile = fopen(filename, "w");
    if (outfile == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        exit(EXIT_FAILURE);
    }

    #pragma omp parallel for
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            #pragma omp critical
            {
                fprintf(outfile, "%d\n", i);
            }
        }
    }

    free(is_prime);
    fclose(outfile);
}

int main() {
    int n;
    printf("Enter limit for prime numbers: ");
    scanf("%d", &n);

    double start_time = omp_get_wtime();
    sieve_of_eratosthenes(n, "prime_numbers.txt");
    double end_time = omp_get_wtime();

    printf("Execution time: %f seconds\n", end_time - start_time);

    return 0;
}

