#include <stdio.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    int n;
    printf("Enter the value of n: ");
    scanf("%d", &n);

    int tc = 4;  //broj tredova

    double start_time = omp_get_wtime();

    int i;
    int sum = 0;

    #pragma omp parallel num_threads(tc) reduction(+:sum)
    {
        int thread_id = omp_get_thread_num();
        int chunk_size = n / tc;
        int start = thread_id * chunk_size + 1;
        int end = (thread_id == tc - 1) ? n : start + chunk_size - 1;

        //parcijalne sume
        int partial_sum = 0;
        for (i = start; i <= end; i++) {
            partial_sum += i;
        }

        printf("Thread %d: Partial Sum = %d (from %d to %d)\n", thread_id, partial_sum, start, end);

        //ukupna suma
        sum += partial_sum;
    }
    
    double end_time = omp_get_wtime();
    double elapsed_time = end_time - start_time;

    printf("Final Sum = %d\n", sum);
    
    printf("Execution Time: %f seconds\n", elapsed_time);
    
    return 0;
}

