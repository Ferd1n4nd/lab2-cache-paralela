#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <cmath>

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
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            double suma = 0.0;
            for (int k = 0; k < n; k++)
                suma += A[i * n + k] * B[k * n + j];
            C[i * n + j] = suma;
        }
    auto fin = steady_clock::now();
    return duration<double>(fin - inicio).count();
}

double multiplicarBloques(const vector<double>& A, const vector<double>& B, vector<double>& C, int n, int bloque) {
    fill(C.begin(), C.end(), 0.0);
    auto inicio = steady_clock::now();
    for (int ii = 0; ii < n; ii += bloque) {
        int limI = min(ii + bloque, n);
        for (int jj = 0; jj < n; jj += bloque) {
            int limJ = min(jj + bloque, n);
            for (int kk = 0; kk < n; kk += bloque) {
                int limK = min(kk + bloque, n);
                for (int i = ii; i < limI; i++)
                    for (int j = jj; j < limJ; j++) {
                        double suma = C[i * n + j];
                        for (int k = kk; k < limK; k++)
                            suma += A[i * n + k] * B[k * n + j];
                        C[i * n + j] = suma;
                    }
            }
        }
    }
    auto fin = steady_clock::now();
    return duration<double>(fin - inicio).count();
}

double diferenciaMaxima(const vector<double>& X, const vector<double>& Y) {
    double dif = 0.0;
    for (size_t i = 0; i < X.size(); i++)
        dif = max(dif, fabs(X[i] - Y[i]));
    return dif;
}

int main() {
    {
        int n = 256;
        vector<double> A((size_t)n * n), B((size_t)n * n);
        vector<double> C1((size_t)n * n), C2((size_t)n * n);
        inicializar(A, B, n);
        multiplicarClasica(A, B, C1, n);
        multiplicarBloques(A, B, C2, n, 32);
        cout << "Verificacion n=256 bloque=32 -> diferencia maxima = "
             << scientific << setprecision(3) << diferenciaMaxima(C1, C2)
             << "\n" << endl;
    }

    vector<int> tamanos = {512, 1024};
    vector<int> bloques = {8, 16, 32, 64, 128};

    ofstream csv("resultados/parte3_bloques.csv");
    csv << "n,version,bloque,tiempo,gflops\n";

    for (int n : tamanos) {
        vector<double> A((size_t)n * n), B((size_t)n * n), C((size_t)n * n);
        inicializar(A, B, n);

        double tClasica = multiplicarClasica(A, B, C, n);
        cout << "n = " << n << "   (clasica: " << fixed << setprecision(6)
             << tClasica << " s)" << endl;
        cout << left << setw(10) << "bloque" << setw(14) << "tiempo(s)"
             << setw(12) << "gflops" << setw(12) << "mejora" << endl;
        cout << string(48, '-') << endl;
        csv << n << ",clasica,0," << tClasica << ","
            << (2.0 * n * n * n) / (tClasica * 1e9) << "\n";

        for (int bloque : bloques) {
            double tiempo = multiplicarBloques(A, B, C, n, bloque);
            double gflops = (2.0 * n * n * n) / (tiempo * 1e9);
            cout << left << setw(10) << bloque
                 << setw(14) << fixed << setprecision(6) << tiempo
                 << setw(12) << setprecision(3) << gflops
                 << setw(12) << setprecision(2) << (tClasica / tiempo) << "x"
                 << endl;
            csv << n << ",bloques," << bloque << "," << tiempo << "," << gflops << "\n";
        }
        cout << endl;
    }

    csv.close();
    return 0;
}