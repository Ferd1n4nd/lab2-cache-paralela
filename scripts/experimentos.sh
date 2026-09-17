set -e

echo "=== Parte 1: bucles anidados ==="
./bin/bucles_anidados
echo

echo "=== Parte 2: multiplicacion clasica ==="
./bin/mult_clasica
echo

echo "=== Parte 3: multiplicacion por bloques ==="
./bin/mult_bloques
echo

echo "CSV generados en resultados/"
ls -1 resultados/*.csv