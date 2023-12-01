#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int returnSize(const char *fname, int csize)
{
    FILE *f = fopen(fname, "r");
    int dim = 0;
    double tmp;
    while (fscanf(f, "%lf ", &tmp) != EOF)
        dim++;
    fclose(f);
    return dim;
}

double *loadVec(const char *fname, int n)
{
    FILE *f = fopen(fname, "r");
    double *res = (double *)malloc(sizeof(double) * n);
    double *it = res;
    while (fscanf(f, "%lf", it++) != EOF)
        ;
    fclose(f);
    return res;
}

double *loadMat(const char *fname, int n, int original_n)
{
    FILE *f = fopen(fname, "r");
    double *res = (double *)malloc(sizeof(double) * original_n * n);
    double *it = res;
    while (fscanf(f, "%lf", it++) != EOF)
        ;
    fclose(f);
    return res;
}

void logRes(const char *fname, double *res, int n)
{
    FILE *f = fopen(fname, "w");
    for (int i = 0; i != n; ++i)
        fprintf(f, "%.2lf ", res[i]);
    fclose(f);
}

int main(int argc, char* argv[])
{
    int csize;
    int prank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &csize);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);

    const char *vfname = "vf.txt";
    const char *mfname = "mf.txt";
    int dim;
    double *mat;
    double *vec;
    double *tmat;
    double *lres;
    double *res;

    if (prank == 0)
        dim = returnSize(vfname, csize);
        
    MPI_Bcast(&dim, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int original_dim = dim;
    int add_in = dim % csize;
    
    if (add_in != 0)
        dim += csize - add_in;

    if (prank == 0)
        vec = loadVec(vfname, dim);
    else
        vec = (double *)malloc(sizeof(double) * dim);
        
    MPI_Bcast(vec, dim, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (prank == 0)
        tmat = loadMat(mfname, dim, original_dim);

    int msize = original_dim * dim / csize;
    mat = (double *)malloc(sizeof(double) * msize);

    MPI_Scatter(tmat, msize, MPI_DOUBLE, mat, msize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int to = dim / csize;
    lres = (double *)malloc(sizeof(double) * to);

    for (int i = 0; i != to; ++i)
    {
        double s = 0;
        for (int j = 0; j != original_dim; ++j)
            s += mat[i * original_dim + j] * vec[j];
        lres[i] = s;
    }

    if (prank == 0)
        res = (double *)malloc(sizeof(double) * dim);

    MPI_Gather(lres, to, MPI_DOUBLE, res, to, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (prank == 0)
    {
        logRes("res.txt", res, original_dim);
    }

    if (prank == 0)
    {
        free(tmat);
        free(res);
    }
    free(vec);
    free(mat);
    free(lres);

    MPI_Finalize();

    return 0;
}

