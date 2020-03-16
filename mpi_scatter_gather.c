/*
MPI Scatter Gather testing program
 main process allocate table with ones
 sends this table to all
 each process multiply table part by rank
 main process gather and print table
*/

#include "mpi.h"
#include <stdio.h>

#define NUMBER_OF_PROCESSES 4

int main(int argc, char **argv) {
    int rank, size, i, j;
    int table[NUMBER_OF_PROCESSES][3];
    int table_recv[NUMBER_OF_PROCESSES][3];
    int row[3];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    /* If I'm the root (process 0), then fill out the big table */
    if (rank == 0) {
        for (j = 0; j < NUMBER_OF_PROCESSES; j++) {
            for (i = 0; i < 3; i++) {
                table[j][i] = 1;
            }
        }
        for (j = 0; j < NUMBER_OF_PROCESSES; j++) {
            for (i = 0; i < 3; i++) {
                printf("%d, ", table[j][i]);
            }
            printf("\n");
        }
        printf("\n");
    }


    /* Scatter the big table to everybody's little table */
    MPI_Scatter(&table[0][0], 3, MPI_INT,
                &row, 3, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = 0; i < 3; i++) {
//        printf("%d, ", row[i]);
        row[i] *= rank;
    }
//    printf("\n\n");


    MPI_Gather(&row, 3, MPI_INT,
               &table_recv[0][0], 3, MPI_INT, 0, MPI_COMM_WORLD);


    if (rank == 0) {
        for (j = 0; j < NUMBER_OF_PROCESSES; j++) {
            for (i = 0; i < 3; i++) {
                printf("%d, ", table_recv[j][i]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}