#include <time.h>
#include <stdlib.h>
#include "pattern_controller.c"
#include "maze-resolution.c"

int main(int argc, char **argv)
{
    int (*matrix)[N] = malloc(sizeof(int[N][N]));
    //getPatternMaze(matrix);
    copy_maze(maze_sizeof_1024, matrix);
    int num_steps = 0;
    printf("Maze with size: %d\n", Size);
    //printf("Initial matrix \n");
    //print_matrix(matrix);
    //double starttime = MPI_Wtime();
    printf("Teste\n");
    while (matrix[N - 2][1] != Goal && num_steps < MAX_STEPS)
    {
        if (DEBUG){
            printf("Continue? (0 == exit)\n\n");
            int choice = -1;
            scanf(" %d", &choice);
        }
        
        mpi_step(matrix);
        //printf("After %d° step \n", num_steps + 1);
        //print_matrix(matrix);
        num_steps++;
    }

    //double endtime = MPI_Wtime();

    //printf("Final matrix: \n");
    //print_matrix(matrix);
    //printf("ELAPSED TIME(OMP) %f sec\n", endtime - starttime);
    printf("TOTAL STEPS: %d\n", num_steps);
}