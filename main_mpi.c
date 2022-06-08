#include <time.h>
#include <stdlib.h>
#include "pattern_controller.c"
#include "maze-resolution.c"

int main(int argc, char **argv)
{
    
    MPI_Init(NULL, NULL);

    int my_rank, n_threads;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if(my_rank == 0{

        int (*matrix)[N] = malloc(sizeof(int[N][N]));
        //getPatternMaze(matrix);
        copy_maze(maze_sizeof_1024, matrix);
        int num_steps = 0;
        printf("Maze with size: %d\n", Size);

        while (matrix[N - 2][1] != Goal && num_steps < MAX_STEPS)
        {
            if (DEBUG){
                printf("Continue? (0 == exit)\n\n");
                int choice = -1;
                scanf(" %d", &choice);
            }
            
            // AQUI ENVIA O QUE CADA PROCESSO VAI CALCULAR
            // MPI SEND / SCATTER

            //printf("After %d° step \n", num_steps + 1);
            //print_matrix(matrix);
            num_steps++;
        }
    } else{
        // AQUI RECEBE O QUE VAI CALCULAR E PASSA PRO STEP
        //MPI RECEIVE / SCATTER
        mpi_step(matrix, my_rank);
    }

    



    //double endtime = MPI_Wtime();

    //printf("Final matrix: \n");
    //print_matrix(matrix);
    //printf("ELAPSED TIME(OMP) %f sec\n", endtime - starttime);
    printf("TOTAL STEPS: %d\n", num_steps);
}