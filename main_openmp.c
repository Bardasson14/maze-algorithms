#include <time.h>
#include <stdlib.h>
#include "pattern_controller.c"
#include "maze-resolution.c"
#include "mpi_utils.c"

int main(void)
{

    int n_threads;

    printf("NÚMERO DE THREADS:\n");
    scanf("%d", &n_threads);

    omp_set_num_threads(n_threads);

    int (*matrix)[N] = malloc(sizeof(int[N][N]));
    // copy_maze(maze_sizeof_1024, matrix);
    copy_maze(maze_sizeof_1024, matrix);


    int num_steps = 0;
    printf("Maze with size: %d\n", Size);
    //printf("Initial matrix \n");
    //print_matrix(matrix);
    double startOMP = omp_get_wtime();
    while (matrix[N - 2][1] != Goal && num_steps < MAX_STEPS)
    {
        if (DEBUG){
            printf("Continue? (0 == exit)\n\n");
            int choice = -1;
            scanf(" %d", &choice);
        }
        ompStep(matrix);
        //printf("After %d° step \n", num_steps + 1);
        //print_matrix(matrix);
        num_steps++;
    }

    double endOMP = omp_get_wtime();

    //printf("Final matrix: \n");
    //print_matrix(matrix);
    printf("ELAPSED TIME(OMP) %f sec\n", endOMP - startOMP);
    printf("TOTAL STEPS: %d\n", num_steps);
}