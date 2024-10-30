#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    int rank, size;
    long long int tosses_per_process, number_in_circle_local = 0, number_in_circle_total = 0;
    long long int total_tosses;
    double x, y, distance_squared, pi_estimate;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Ingrese el número total de lanzamientos (dardos): ");
        scanf("%lld", &total_tosses);
    }
    MPI_Bcast(&total_tosses, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    tosses_per_process = total_tosses / size;

    srand(time(NULL) + rank);

    for (long long int toss = 0; toss < tosses_per_process; toss++) {
        x = (double)rand() / RAND_MAX * 2.0 - 1.0; 
        y = (double)rand() / RAND_MAX * 2.0 - 1.0;  
        distance_squared = x * x + y * y;
        
        if (distance_squared <= 1) {
            number_in_circle_local++;
        }
    }

    MPI_Reduce(&number_in_circle_local, &number_in_circle_total, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        pi_estimate = 4.0 * number_in_circle_total / ((double) total_tosses);
        printf("Estimación de pi: %f\n", pi_estimate);
    }

    MPI_Finalize();
    return 0;
}
