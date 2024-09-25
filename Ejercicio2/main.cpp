#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void multiplicarMatriz(int **a, int **b, int **result, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            result[i][j] = 0;
            for (int k = 0; k < n; k++)
            {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

// Función para generar matrices aleatorias
void llenarMatriz(int **matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = rand() % 1000; // Números aleatorios entre 0 y 999
        }
    }
}

// Función para crear una matriz dinámica
int **crearMatriz(int n)
{
    int **matrix = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        matrix[i] = (int *)malloc(n * sizeof(int));
    }
    return matrix;
}

// Función para liberar la memoria de una matriz
void eliminarMatriz(int **matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

int main()
{
    srand(time(0)); // Semilla para números aleatorios

    int n[] = {100, 200, 500, 1000, 5000,10000,50000}; // Diferentes tamaños de matrices
    int num_tests = sizeof(n) / sizeof(n[0]);

    for (int t = 0; t < num_tests; t++)
    {
        int size = n[t];

        // Crear matrices
        int **a = crearMatriz(size);
        int **b = crearMatriz(size);
        int **result = crearMatriz(size);

        // Llenar matrices con valores aleatorios
        llenarMatriz(a, size);
        llenarMatriz(b, size);

        // Medir el tiempo de inicio
        clock_t start = clock();

        // Multiplicar matrices
        multiplicarMatriz(a, b, result, size);

        // Medir el tiempo de fin
        clock_t end = clock();

        // Calcular el tiempo en segundos
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

        // Mostrar el tamaño y el tiempo de ejecución
        printf("Tamaño de la matriz: %d x %d, Tiempo de ejecución: %.6f segundos\n", size, size, time_spent);

        // Liberar memoria
        eliminarMatriz(a, size);
        eliminarMatriz(b, size);
        eliminarMatriz(result, size);
    }

    return 0;
}
