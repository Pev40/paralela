#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int rank, comm_sz;
    int local_value, partner_rank;
    int sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    // Inicializar valor local (por ejemplo, el rango de cada proceso)
    local_value = rank;
    sum = local_value;

    // Si el número de procesos no es potencia de dos, manejar los procesos sobrantes
    int active_procs = comm_sz;
    while (active_procs & (active_procs - 1)) {  // Mientras no sea potencia de dos
        if (rank >= active_procs / 2 && rank < active_procs) {
            MPI_Send(&sum, 1, MPI_INT, rank - active_procs / 2, 0, MPI_COMM_WORLD);
        } else if (rank < active_procs / 2) {
            int received_value;
            MPI_Recv(&received_value, 1, MPI_INT, rank + active_procs / 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += received_value;
        }
        active_procs /= 2;
    }

    // Proceso de suma en árbol (número de procesos ahora es potencia de dos)
    for (int step = 1; step < active_procs; step *= 2) {
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

    // El proceso 0 imprime la suma global
    if (rank == 0) {
        printf("La suma global es: %d\n", sum);
    }

    MPI_Finalize();
    return 0;
}
