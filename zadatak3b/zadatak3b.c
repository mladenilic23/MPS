#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define VECTOR_SIZE 10

int main(int argc, char** argv) {
    int prank, csize;
    double vector_a[VECTOR_SIZE];
    double vector_b[VECTOR_SIZE];
    double result[VECTOR_SIZE];
    double local_sum = 0.0;
    double global_sum = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    MPI_Comm_size(MPI_COMM_WORLD, &csize);

    if (prank == 0) {
        // Generisanje vektora
        srand(time(NULL));
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vector_a[i] = (double)(rand() % 100);
            vector_b[i] = (double)(rand() % 100);
        }
    }

    MPI_Bcast(vector_a, VECTOR_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector_b, VECTOR_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double s = MPI_Wtime();
    
    // Ciklična raspodela
    for (int i = prank; i < VECTOR_SIZE; i += csize) {
        result[i] = vector_a[i] * vector_b[i];
        local_sum += result[i];
    }

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double e = MPI_Wtime();
    double d = e - s;
    double mind;

 
    MPI_Reduce(&d, &mind, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (prank == 0) {
        // Ispisivanje na procesu 0
        printf("Vector A: ");
        for (int i = 0; i < VECTOR_SIZE; i++) {
            printf("%.1f ", vector_a[i]);
        }
        printf("\n");

        printf("Vector B: ");
        for (int i = 0; i < VECTOR_SIZE; i++) {
            printf("%.1f ", vector_b[i]);
        }
        printf("\n");

        printf("Sum of results: %.1f\n", global_sum);

        printf("Elapsed time: %f seconds\n", d);
    }

    MPI_Finalize();
    return 0;
}

