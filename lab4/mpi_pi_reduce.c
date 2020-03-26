#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

double rand_0_1() {
    return (double) rand() / (double) RAND_MAX;
}

double vec_length(double x, double y) {
    return sqrt(x * x + y * y);
}

int main(int argc, char **argv) {
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
           processor_name, world_rank, world_size);

    double x, y;
    unsigned long in_circle, n;
    char *file_name;

    time_t tt;
    int seed = time(&tt);
    srand(seed);
    in_circle = 0;

    if (argv[1] && argv[2]) {
        n = strtoul(argv[1], NULL, 10);
        n /= world_size;
        file_name = argv[2];
    } else {
        printf("Wrong program argument");
        exit(1);
    }

    double start_time;

    MPI_Barrier(MPI_COMM_WORLD);
    if (world_rank == 0)
        start_time = MPI_Wtime();
    for (long i = 0; i < n; ++i) {
        x = rand_0_1();
        y = rand_0_1();
        if (vec_length(x, y) <= 1) {
            in_circle++;
        }
    }

    // Reduce all of the local sums into the global sum
    unsigned long global_in_circle_sum;
    MPI_Reduce(&in_circle, &global_in_circle_sum, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0,
               MPI_COMM_WORLD);

    if (world_rank == 0) {
        double time = MPI_Wtime() - start_time;

        printf("time = %gs\n", time);
        double pi = (double) 4.0 * global_in_circle_sum / (n * world_size);
        printf("n = %lu, pi = %f\n", n * world_size, pi);

        FILE *fp;
        if ((fp = fopen(file_name, "a")) == NULL) {
            printf("Can't open data.csv in append mode!\n");
            exit(1);
        }
        fprintf(fp, "%lu, %i, %g, %g", n * world_size, world_size, time, pi);
        fprintf(fp, "\n");
        fclose(fp);
    }

    MPI_Finalize();
    return 0;
}