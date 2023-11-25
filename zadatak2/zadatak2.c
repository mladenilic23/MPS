#include <stdio.h>
#include <mpi.h>

double getInput() {
    double res;
    printf("Number: ");
    fflush(stdout);
    scanf("%lf", &res);
    return (double)(res);
}

int main(int argc, char* argv[]) {
    double n;
    double sum = 0;
    int csize, prank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &csize);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);

    if (prank == 0) {
        n = getInput();
    }

    MPI_Bcast(&n, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double s = MPI_Wtime();
    double ds = (double)csize;
    double chunk_size = n / ds;
    double start = chunk_size * (double)prank + 1;
    double end = (prank == csize - 1) ? n : start + chunk_size - 1;

    for (double i = start; i <= end; i++) {
        sum += i;
    }

    double tsum;

    MPI_Reduce(&sum, &tsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double e = MPI_Wtime();
    double d = e - s;
    double mind;

    MPI_Reduce(&d, &mind, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (prank == 0) {
        printf("Sum first %f integer is %f\n", n, tsum);
        printf("Elapsed time: %f\n", d);
    }

    MPI_Finalize();

    return 0;
}

