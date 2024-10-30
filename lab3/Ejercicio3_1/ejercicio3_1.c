#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void calculate_histogram(int* data, int data_count, int bin_count, int min_meas, int max_meas, int* histogram) {
    float bin_width = (float)(max_meas - min_meas) / bin_count;
    for (int i = 0; i < data_count; i++) {
        int bin = (data[i] - min_meas) / bin_width;
        if (bin >= 0 && bin < bin_count) {
            histogram[bin]++;
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int min_meas = 0, max_meas = 5, bin_count = 5;
    int data_count = 10000;
    int* data = NULL;

    if (rank == 0) {
        data = (int*)malloc(data_count * sizeof(int));
        srand(time(NULL));
        for (int i = 0; i < data_count; i++) {
            data[i] = min_meas + rand() % (max_meas + 1);
        }
    }

    int local_data_count = data_count / size;
    int* local_data = (int*)malloc(local_data_count * sizeof(int));
    MPI_Scatter(data, local_data_count, MPI_INT, local_data, local_data_count, MPI_INT, 0, MPI_COMM_WORLD);

    int* local_histogram = (int*)calloc(bin_count, sizeof(int));
    calculate_histogram(local_data, local_data_count, bin_count, min_meas, max_meas, local_histogram);

    int* full_histogram = NULL;
    if (rank == 0) {
        full_histogram = (int*)calloc(bin_count, sizeof(int));
    }
    MPI_Reduce(local_histogram, full_histogram, bin_count, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        FILE* file = fopen("histogram_data.txt", "w");
        for (int i = 0; i < bin_count; i++) {
            fprintf(file, "%d\n", full_histogram[i]);
        }
        fclose(file);
        printf("Datos guardados en 'histogram_data.txt'.\n");
        free(full_histogram);
    }

    if (rank == 0) free(data);
    free(local_data);
    free(local_histogram);

    MPI_Finalize();
    return 0;
}
