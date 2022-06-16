# maze-algorithms

SEQUENCIAL
mpicc main_seq.c -o main_seq -fopenmp -lm -w
./main_seq

OPEN MP
mpicc main_openmp.c -o main_openmp -fopenmp -lm -w
./main_openmp