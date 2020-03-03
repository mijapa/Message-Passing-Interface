/*
  MPI Delay Test Program
*/
#include <stdio.h>
#include <mpi.h>

#define PING_PONG_LIMIT 1000000

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

    int ping_pong_count = 0;
    int partner_rank = (world_rank + 1) % 2;
    double start_time = 0;

    if (world_rank == 0) {
        start_time = MPI_Wtime();
    }

    MPI_Barrier(MPI_COMM_WORLD);

    while (ping_pong_count < PING_PONG_LIMIT) {

        if (world_rank == ping_pong_count % 2) {
            // Increment the ping pong count before you send it
            ping_pong_count++;
            MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0,
                     MPI_COMM_WORLD);
//            printf("%d sent and incremented ping_pong_count "
//                   "%d to %d\n", world_rank, ping_pong_count,
//                   partner_rank);
        } else {
            MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//            printf("%d received ping_pong_count %d from %d\n",
//                   world_rank, ping_pong_count, partner_rank);

        }
    }

    if (world_rank == 0) {
        double time = MPI_Wtime() - start_time;
        printf("time = %gs\n", time);
        printf("RTT = %gus\n", time / PING_PONG_LIMIT * 1000 * 1000);
    }

    MPI_Finalize();
    return 0;
}