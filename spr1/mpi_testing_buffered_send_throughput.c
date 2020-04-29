/*
MPI Standard Send Throughput Test Program
*/

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#define PING_PONG_LIMIT (1000*1000)

#define TAB_SIZE (1)

int main(int argc, char **argv) {
    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // We are assuming 2 processes for this task
    if (world_size != 2) {
        fprintf(stderr, "World size must be 2 for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
           processor_name, world_rank, world_size);

    int partner_rank = (world_rank + 1) % 2;
    double start_time = 0;

    if (world_rank == 0) {
        start_time = MPI_Wtime();
    }
    int ping_pong_tab[TAB_SIZE] = {0};
    // Declare the buffer and attach it
    int buffer_attached_size = MPI_BSEND_OVERHEAD + sizeof(int) * TAB_SIZE;
    char *buffer_attached = (char *) malloc(buffer_attached_size);
    MPI_Buffer_attach(buffer_attached, buffer_attached_size);

    MPI_Barrier(MPI_COMM_WORLD);

    while (ping_pong_tab[0] < PING_PONG_LIMIT) {

        if (world_rank == ping_pong_tab[0] % 2) {
            // Increment the ping pong count before you send it
            ping_pong_tab[0]++;
            MPI_Bsend(&ping_pong_tab, TAB_SIZE, MPI_INT, partner_rank, 0,
                      MPI_COMM_WORLD);
//            printf("%d sent and incremented ping_pong_count "
//                   "%d to %d\n", world_rank, ping_pong_count,
//                   partner_rank);
        } else {
            MPI_Recv(&ping_pong_tab, TAB_SIZE, MPI_INT, partner_rank, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//            printf("%d received ping_pong_count %d from %d\n",
//                   world_rank, ping_pong_count, partner_rank);

        }
    }

    MPI_Buffer_detach(&buffer_attached, &buffer_attached_size);
    free(buffer_attached);

    MPI_Barrier(MPI_COMM_WORLD);

    if (world_rank == 0) {
        double time = MPI_Wtime() - start_time;
        printf("time = %gs\n", time);
        double size = 1.0 * sizeof(int) * TAB_SIZE;
        printf("message size = %g B\n", size);
        printf("hop time = %g us\n", time / PING_PONG_LIMIT * 1000 * 1000);
        printf("throughput = %g Mb/s\n", size * 8 / (1024 * 1024) / (time / PING_PONG_LIMIT));
    }

    MPI_Finalize();
    return 0;
}