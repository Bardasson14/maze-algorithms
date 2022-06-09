#include <time.h>
#include <stdlib.h>
#include "pattern_controller.c"
#include "maze-resolution.c"

int main(int argc, char **argv)
{
    // Inicia MPI
    int initialized, finalized, rank, size;
    MPI_Initialized(&initialized);
    if (!initialized)
        MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double total_time = 0;
    double partial_time = 0;
    int num_steps = 0;
    int (*matrix)[N];

    if (rank == 0){
        matrix = malloc(sizeof(int[N][N]));
        getPatternMaze(matrix);

        MPI_Bcast(matrix, N*N, MPI_INT, 0, MPI_COMM_WORLD);
        num_steps = 0;
        printf("Maze with size: %d\n", Size);
        //printf("Initial matrix \n");
        //print_matrix(matrix);
    }
        
    while (matrix[N - 2][1] != Goal && num_steps < MAX_STEPS)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0){
            if (DEBUG){
                printf("Continue? (0 == exit)\n\n");
                int choice = -1;
                scanf(" %d", &choice);
            }
        }
    
        partial_time = mpi_step(matrix);

        if (rank == 0){
            // recebe mensagem com o tempo tomado
            printf("After %d° step \n", num_steps + 1);
            print_matrix(matrix);
            total_time += partial_time;
            num_steps++;
        }
    }

    if (rank == 0){
        //printf("Final matrix: \n");
        //print_matrix(matrix);
        printf("ELAPSED TIME(MPI) %f sec\n", total_time);
        printf("TOTAL STEPS: %d\n", num_steps);
    }
    
    MPI_Finalize();
    return 0;
}