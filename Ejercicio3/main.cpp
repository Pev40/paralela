#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void multiplyMatricesClassic(int **a, int **b, int **result, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = 0;
            for (int k = 0; k < n; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void multiplicacionBloques(int **a, int **b, int **result, int n, int blockSize) {
    for (int i = 0; i < n; i += blockSize) {
        for (int j = 0; j < n; j += blockSize) {
            for (int k = 0; k < n; k += blockSize) {
                for (int ii = i; ii < i + blockSize && ii < n; ii++) {
                    for (int jj = j; jj < j + blockSize && jj < n; jj++) {
                        for (int kk = k; kk < k + blockSize && kk < n; kk++) {
                            result[ii][jj] += a[ii][kk] * b[kk][jj];
                        }
                    }
                }
            }
        }
    }
}

void llenarMatriz(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 10; 
        }
    }
}

int** crearMatriz(int n) {
    int **matrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(n * sizeof(int));
    }
    return matrix;
}

void librerarMemoria(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    srand(time(0)); 

    int n[] = {100, 200, 500, 1000,5000,10000,50000};  // Diferentes tamaños de matrices para probar
    int num_tests = sizeof(n) / sizeof(n[0]);
    int blockSizes[] = {16, 32, 64, 128,256};  // Diferentes tamaños de bloques a probar
    int num_block_sizes = sizeof(blockSizes) / sizeof(blockSizes[0]);

    FILE *file = fopen("resultados.csv", "w");
    fprintf(file, "Matrix Size,Block Size,Time Block (s)\n");

    for (int t = 0; t < num_tests; t++) {
        int size = n[t];

        for (int b = 0; b < num_block_sizes; b++) {
            int blockSize = blockSizes[b];

            // Crear matrices
            int **a = crearMatriz(size);
            int **b_matrix = crearMatriz(size);
            int **result_block = crearMatriz(size);

            // Llenar matrices con valores aleatorios
            llenarMatriz(a, size);
            llenarMatriz(b_matrix, size);

            // Medir el tiempo para la multiplicación por bloques
            clock_t start_block = clock();
            multiplicacionBloques(a, b_matrix, result_block, size, blockSize);
            clock_t end_block = clock();
            double time_block = (double)(end_block - start_block) / CLOCKS_PER_SEC;

            // Guardar los resultados en el archivo
            fprintf(file, "%d,%d,%.6f\n", size, blockSize, time_block);

            // Liberar memoria
            librerarMemoria(a, size);
            librerarMemoria(b_matrix, size);
            librerarMemoria(result_block, size);
        }
    }

    fclose(file);
    return 0;
}
