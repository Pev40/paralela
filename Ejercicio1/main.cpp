#include <iostream>
#include <vector>
#include <chrono>

int main() {
    const int rows = 3;
    const int cols = 3;
    std::vector<std::vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    // Primer bucle for (por filas)
    auto start1 = std::chrono::high_resolution_clock::now();
    long long sum1 = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << matrix[i][j] << " ";
            sum1 += matrix[i][j];
        }
        std::cout << std::endl; // Añadir un salto de línea después de cada fila
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration1 = end1 - start1;

    // Segundo bucle for (por columnas)
    auto start2 = std::chrono::high_resolution_clock::now();
    long long sum2 = 0;
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows; ++i) {
            sum2 += matrix[i][j];
        }
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration2 = end2 - start2;

    // Resultados
    std::cout << "Suma con primer bucle for: " << sum1 << " en " << duration1.count() << " segundos.\n";
    std::cout << "Suma con segundo bucle for: " << sum2 << " en " << duration2.count() << " segundos.\n";

    return 0;
}