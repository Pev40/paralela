#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, comm_sz;
    int local_value, partner_rank;
    int sum = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    if ((comm_sz & (comm_sz - 1)) != 0) {
        if (rank == 0) {
            printf("El número de procesos debe ser una potencia de dos.\n");
        }
        MPI_Finalize();
        return 1;
    }

    local_value = rank;
    sum = local_value;

    for (int step = 1; step < comm_sz; step *= 2) {
        if (rank % (2 * step) == 0) {
            partner_rank = rank + step;
            int received_value;
            MPI_Recv(&received_value, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += received_value;
        } else if (rank % step == 0) {
            partner_rank = rank - step;
            MPI_Send(&sum, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
            break;
        }
    }
    if (rank == 0) {
        printf("La suma global es: %d\n", sum);
    }

    MPI_Finalize();
    return 0;
}
