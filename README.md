# maze-algorithms
# para executar com maze_sizeof_Size, é necessário mudar N em constants.h

SEQUENCIAL
mpicc main_seq.c -o main_seq -fopenmp -lm -w
./main_seq

OPEN MP
mpicc main_openmp.c -o main_openmp -fopenmp -lm -w
./main_openmp

MPI
mpicc main_mpi.c -o main_mpi -fopenmp -lm -w
mpiexec -np 5 --oversubscribe main_mpi