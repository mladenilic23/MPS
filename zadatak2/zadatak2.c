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
    int n;
    int sum = 0;
    int csize, prank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &csize);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);

    if (prank == 0) {
        n = getInput();
    }

    MPI_Bcast(&n, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double s = MPI_Wtime();
    
    int width = n/csize;  
    int left_out = csize - n % csize; //ostatak

    int pos;  //pozicija svakog procesa

    if(prank == 0)
        pos = 1;
    else if(prank < left_out)
        pos = width + 1 + (prank - 1)*width;
    else if(prank == left_out)
        pos = width*left_out + 1;
    else if(prank > left_out)
        pos = 1 + width*prank + prank - left_out;

        
    if(left_out != csize)
        if(prank >= left_out)
            width++;

    int i = 0;

    while (i < width){
        sum += pos;
        pos++;
        i++;
    }
    
    int tsum;

    MPI_Reduce(&sum, &tsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double e = MPI_Wtime();
    double d = e - s;
    double mind;

    MPI_Reduce(&d, &mind, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (prank == 0) {
        printf("Sum first %d integer is %d\n", n, tsum);
        printf("Elapsed time: %f\n", d);
    }

    MPI_Finalize();

    return 0;
}

