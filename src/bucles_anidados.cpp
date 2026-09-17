#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace std::chrono;

void inicializar(vector<double>& A, vector<double>& x, int n) {
    for (int i = 0; i < n; i++) {
        x[i] = 1.0 + (i % 5);
        for (int j = 0; j < n; j++)
            A[i * n + j] = 1.0 + ((i + j) % 7);
    }
}

double porFilas(const vector<double>& A, const vector<double>& x, vector<double>& y, int n) {
    fill(y.begin(), y.end(), 0.0);
    auto inicio = steady_clock::now();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            y[i] += A[i * n + j] * x[j];
    auto fin = steady_clock::now();
    return duration<double>(fin - inicio).count();
}

double porColumnas(const vector<double>& A, const vector<double>& x, vector<double>& y, int n) {
    fill(y.begin(), y.end(), 0.0);
    auto inicio = steady_clock::now();
    for (int j = 0; j < n; j++)
        for (int i = 0; i < n; i++)
            y[i] += A[i * n + j] * x[j];
    auto fin = steady_clock::now();
    return duration<double>(fin - inicio).count();
}

int main() {
    const int repeticiones = 3;
    vector<int> tamanos = {512, 1024, 2048, 4096};

    ofstream csv("resultados/parte1_bucles.csv");
    csv << "n,iteraciones,tiempo_filas,tiempo_columnas,razon\n";

    cout << left << setw(8) << "n" << setw(16) << "iteraciones"
         << setw(14) << "filas(s)" << setw(14) << "columnas(s)"
         << setw(10) << "razon" << endl;
    cout << string(62, '-') << endl;

    for (int n : tamanos) {
        vector<double> A((size_t)n * n), x(n), y(n);
        inicializar(A, x, n);

        double mejorFilas = 1e30, mejorCols = 1e30;
        for (int r = 0; r < repeticiones; r++) {
            mejorFilas = min(mejorFilas, porFilas(A, x, y, n));
            mejorCols  = min(mejorCols,  porColumnas(A, x, y, n));
        }
        double razon = mejorCols / mejorFilas;

        cout << left << setw(8) << n << setw(16) << (long long)n * n
             << setw(14) << fixed << setprecision(6) << mejorFilas
             << setw(14) << mejorCols
             << setw(10) << setprecision(2) << razon << "x" << endl;

        csv << n << "," << (long long)n * n << ","
            << setprecision(6) << mejorFilas << "," << mejorCols << ","
            << setprecision(3) << razon << "\n";
    }

    csv.close();
    return 0;
}