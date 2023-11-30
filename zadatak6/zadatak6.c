#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void eratostenovo_sito(int n, const char* filename, int chunk_size, int dynamic_flag, int guided_flag) {
    
    char* is_prime = (char*)malloc((n + 1) * sizeof(char));

    #pragma omp parallel for
    for (int i = 0; i <= n; i++) {
        is_prime[i] = 1;
    }

    is_prime[0] = is_prime[1] = 0;

    #pragma omp parallel for schedule(dynamic, chunk_size) if(dynamic_flag)
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            if (guided_flag) {
                #pragma omp parallel for schedule(guided)
                for (int j = i * i; j <= n; j += i) {
                    is_prime[j] = 0;
                }
            } else {
                #pragma omp parallel for schedule(static, chunk_size) if(!dynamic_flag)
                for (int j = i * i; j <= n; j += i) {
                    is_prime[j] = 0;
                }
            }
        }
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    #pragma omp parallel for
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            #pragma omp critical
            {
                fprintf(file, "%d\n", i);
            }
        }
    }

    fclose(file);
    free(is_prime);
}

int main() {
    int n, chunk_size;
    printf("Limit for prime numbers: ");
    scanf("%d", &n);
    printf("Enter the chunk_size: ");
    scanf("%d", &chunk_size);

    double start_time_1, start_time_2, start_time_3, end_time_1, end_time_2, end_time_3;

    //Strategy 1: Static 
    start_time_1 = omp_get_wtime();
    eratostenovo_sito(n, "primes_static.txt", chunk_size, 0, 0);
    end_time_1 = omp_get_wtime();
    printf("Static scheduling execution time: %fs\n", end_time_1 - start_time_1);

    //Strategy 2: Dynamic 
    start_time_2 = omp_get_wtime();
    eratostenovo_sito(n, "primes_dynamic.txt", chunk_size, 1, 0);
    end_time_2 = omp_get_wtime();
    printf("Dynamic scheduling execution time: %fs\n", end_time_2 - start_time_2);

    //Strategy 3: Guided 
    start_time_3 = omp_get_wtime();
    eratostenovo_sito(n, "primes_guided.txt", chunk_size, 1, 1);
    end_time_3 = omp_get_wtime();
    printf("Guided scheduling execution time: %fs\n", end_time_3 - start_time_3);

    return 0;
}

