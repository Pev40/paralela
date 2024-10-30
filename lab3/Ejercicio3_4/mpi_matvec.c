#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void matrix_vector_multiply(double* local_A, double* local_x, double* local_y, int block_size) {
    for (int i = 0; i < block_size; i++) {
        local_y[i] = 0.0;
        for (int j = 0; j < block_size; j++) {
            local_y[i] += local_A[i * block_size + j] * local_x[j];
        }
    }
}

int main(int argc, char** argv) {
    int rank, comm_sz;
    int matrix_order = 4; 
    int block_size; 

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    int q = (int)sqrt(comm_sz);
    if (q * q != comm_sz || matrix_order % q != 0) {
        if (rank == 0) {
            printf("El número de procesos debe ser un cuadrado perfecto y dividir el orden de la matriz.\n");
        }
        MPI_Finalize();
        return 1;
    }

    block_size = matrix_order / q;

    double* A = NULL;     
    double* x = NULL;   
    double* local_A = (double*)malloc(block_size * block_size * sizeof(double));
    double* local_x = (double*)malloc(block_size * sizeof(double));
    double* local_y = (double*)malloc(block_size * sizeof(double));

    if (rank == 0) {
        A = (double*)malloc(matrix_order * matrix_order * sizeof(double));
        x = (double*)malloc(matrix_order * sizeof(double));
        for (int i = 0; i < matrix_order; i++) {
            x[i] = 1.0;  
            for (int j = 0; j < matrix_order; j++) {
                A[i * matrix_order + j] = i * matrix_order + j + 1; 
            }
        }
    }
    if (rank == 0) {
        for (int i = 0; i < q; i++) {
            for (int j = 0; j < q; j++) {
                int dest_rank = i * q + j;
                if (dest_rank == 0) {
                    for (int k = 0; k < block_size; k++)
                        for (int l = 0; l < block_size; l++)
                            local_A[k * block_size + l] = A[k * matrix_order + l];
                } else {
                    MPI_Send(&A[(i * block_size * matrix_order) + j * block_size], block_size * block_size, MPI_DOUBLE, dest_rank, 0, MPI_COMM_WORLD);
                }
            }
        }
    } else {
        MPI_Recv(local_A, block_size * block_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    if (rank % (q + 1) == 0) {
        if (rank == 0) {
            for (int i = 1; i < q; i++) {
                MPI_Send(&x[i * block_size], block_size, MPI_DOUBLE, i * (q + 1), 0, MPI_COMM_WORLD);
            }
            for (int i = 0; i < block_size; i++) {
                local_x[i] = x[i];
            }
        } else {
            MPI_Recv(local_x, block_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    matrix_vector_multiply(local_A, local_x, local_y, block_size);
    double* y = NULL;
    if (rank == 0) {
        y = (double*)malloc(matrix_order * sizeof(double));
    }
    MPI_Gather(local_y, block_size, MPI_DOUBLE, y, block_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Resultado de la multiplicación matriz-vector:\n");
        for (int i = 0; i < matrix_order; i++) {
            printf("%f ", y[i]);
        }
        printf("\n");
        free(A);
        free(x);
        free(y);
    }

    free(local_A);
    free(local_x);
    free(local_y);

    MPI_Finalize();
    return 0;
}
