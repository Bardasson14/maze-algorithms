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

    double starttime, endtime;
    double total_time = 0;
    int num_steps = 0;
    int stopSteps = 0;
    int (*matrix)[N];

    matrix = malloc(sizeof(int[N][N]));
    if (rank == 0){
        getPatternMaze(matrix);

        num_steps = 0;
        printf("Maze with size: %d\n", Size);
        //printf("Initial matrix \n");
        //print_matrix(matrix);
    }
    MPI_Bcast(matrix, N*N, MPI_INT, 0, MPI_COMM_WORLD);
    // Sincroniza relógio
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0){
        starttime = MPI_Wtime();
    }
        
    while (matrix[N - 2][1] != Goal && num_steps < MAX_STEPS && !stopSteps)
    {
        if (rank == 0){
            if (DEBUG){
                printf("Continue? (0 == exit)\n\n");
                int choice = -1;
                scanf(" %d", &choice);
            }
        }

        mpi_step(matrix);

        if (rank == 0){
            //printf("After %d° step \n", num_steps + 1);
            //print_matrix(matrix);
            num_steps++;

            // Avisa aos trabalhadores que acabou while
            if (matrix[N - 2][1] == Goal || num_steps >= MAX_STEPS){
                stopSteps = 1;
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&stopSteps, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Sincroniza relógio
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0){
        endtime = MPI_Wtime();
        total_time = endtime - starttime;
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