set -e
mkdir -p resultados/cachegrind
N=256
BLOQUE=32
DIR=resultados/cachegrind

echo ">>> Parte 1: recorrido por filas"
valgrind --tool=cachegrind --cache-sim=yes \
  --cachegrind-out-file=$DIR/cg_filas.out \
  ./bin/bucles_anidados 1024 filas
cg_annotate $DIR/cg_filas.out > $DIR/filas.txt

echo ">>> Parte 1: recorrido por columnas"
valgrind --tool=cachegrind --cache-sim=yes \
  --cachegrind-out-file=$DIR/cg_columnas.out \
  ./bin/bucles_anidados 1024 columnas
cg_annotate $DIR/cg_columnas.out > $DIR/columnas.txt

echo ">>> Multiplicacion clasica (n=$N)"
valgrind --tool=cachegrind --cache-sim=yes \
  --cachegrind-out-file=$DIR/cg_clasica.out \
  ./bin/mult_clasica $N
cg_annotate $DIR/cg_clasica.out > $DIR/clasica.txt

echo ">>> Multiplicacion por bloques (n=$N, bloque=$BLOQUE)"
valgrind --tool=cachegrind --cache-sim=yes \
  --cachegrind-out-file=$DIR/cg_bloques.out \
  ./bin/mult_bloques $N $BLOQUE
cg_annotate $DIR/cg_bloques.out > $DIR/bloques.txt

echo
echo "Resumenes de texto en $DIR/*.txt"
echo "Para visualizar:  kcachegrind $DIR/cg_clasica.out &"