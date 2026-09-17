#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <fstream>
#include <algorithm>

using namespace std;
using namespace std::chrono;

void inicializar(vector<double>& A, vector<double>& B, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            A[i * n + j] = 1.0 + ((i + j) % 9);
            B[i * n + j] = 1.0 + ((i * 2 + j) % 5);
        }
}

double multiplicarClasica(const vector<double>& A, const vector<double>& B, vector<double>& C, int n) {
    fill(C.begin(), C.end(), 0.0);
    auto inicio = steady_clock::now();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double suma = 0.0;
            for (int k = 0; k < n; k++)
                suma += A[i * n + k] * B[k * n + j];
            C[i * n + j] = suma;
        }
    }
    auto fin = steady_clock::now();
    return duration<double>(fin - inicio).count();
}

int main() {
    vector<int> tamanos = {128, 256, 512, 768, 1024};

    ofstream csv("resultados/parte2_clasica.csv");
    csv << "n,operaciones,tiempo,gflops,factor\n";

    cout << left << setw(8) << "n" << setw(16) << "operaciones"
         << setw(14) << "tiempo(s)" << setw(12) << "gflops"
         << setw(10) << "factor" << endl;
    cout << string(60, '-') << endl;

    double anterior = 0.0;
    for (int n : tamanos) {
        vector<double> A((size_t)n * n), B((size_t)n * n), C((size_t)n * n);
        inicializar(A, B, n);

        double tiempo = multiplicarClasica(A, B, C, n);
        double operaciones = 2.0 * n * n * n;
        double gflops = operaciones / (tiempo * 1e9);
        double factor = (anterior > 0.0) ? tiempo / anterior : 0.0;

        cout << left << setw(8) << n << setw(16) << scientific
             << setprecision(2) << operaciones
             << setw(14) << fixed << setprecision(6) << tiempo
             << setw(12) << setprecision(3) << gflops;
        if (factor > 0.0) cout << setw(10) << setprecision(2) << factor << "x";
        cout << endl;

        csv << n << "," << operaciones << "," << tiempo << "," << gflops << "," << factor << "\n";
        anterior = tiempo;
    }

    csv.close();
    return 0;
}