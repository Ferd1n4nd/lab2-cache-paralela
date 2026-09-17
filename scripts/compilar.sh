set -e
mkdir -p bin resultados/cachegrind

FLAGS="-O2 -g -std=c++17 -Wall"

g++ $FLAGS -o bin/bucles_anidados src/bucles_anidados.cpp
g++ $FLAGS -o bin/mult_clasica    src/mult_clasica.cpp
g++ $FLAGS -o bin/mult_bloques    src/mult_bloques.cpp

echo "Compilacion terminada. Ejecutables en bin/"