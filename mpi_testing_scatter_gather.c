/*
MPI Scatter Gather testing program
    testing scatter and gather local and global time with big buffer
*/
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define  VAL (1024*1024*10)

int main(int argc, char **argv) {
    int rank, size;


    // Create a buffer that will hold a subset of the random numbers
    float *buff = malloc(sizeof(float) * VAL * 4);
    float *buff2 = malloc(sizeof(float) * VAL);
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    if (rank == 0) {
        printf(" buff size: %gMB \n", 1.0 * VAL * sizeof(float) / (1024 * 1024));
    }

    /* If I'm the root (process 0), then fill out the big table */

    double start_time = MPI_Wtime();
    /* Scatter the big table to everybody's little table */
    MPI_Scatter(buff, VAL, MPI_FLOAT,
                buff2, VAL, MPI_FLOAT, 0, MPI_COMM_WORLD);

    printf("scatter in process: %d, time = %gs\n", rank, MPI_Wtime() - start_time);

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        printf("scatter global time = %gs\n", MPI_Wtime() - start_time);
    }

    start_time = MPI_Wtime();
    MPI_Gather(buff2, VAL, MPI_FLOAT,
               buff, VAL, MPI_FLOAT, 0, MPI_COMM_WORLD);
    printf("gather in process: %d, time = %gs\n", rank, MPI_Wtime() - start_time);

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        printf("gather global time = %gs\n", MPI_Wtime() - start_time);
    }

    MPI_Finalize();
    return 0;
}