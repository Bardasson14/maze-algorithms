#include <time.h>
#include <stdlib.h>
#include "pattern_controller.c"
#include "maze-resolution.c"

int main(int argc, char **argv)
{

    int thread_id, n_threads, size;

    printf("NÚMERO DE THREADS:\n");
    scanf("%d", &n_threads);

    omp_set_num_threads(n_threads);

    int (*matrix)[N] = malloc(sizeof(int[N][N]));
    int (*temp_matrix)[N] = malloc(sizeof(int[N][N]));
    //int matrix[N][N] = {0}; 
    //int temp_matrix[N][N] = {0}; 
    getPatternMaze(matrix);

    int num_steps = 0;
    printf("Initial matrix \n");
    //print_matrix(matrix);
    int start = clock();
    double startOMP = omp_get_wtime();
    while (matrix[N - 2][1] != Goal && num_steps < MAX_STEPS)
    {
        if (DEBUG){
            printf("Continue? (0 == exit)\n\n");
            int choice = -1;
            scanf(" %d", &choice);
        }
        make_zeros(temp_matrix);
        parallel_step(matrix, temp_matrix);
        printf("After %d° step \n", num_steps + 1);
        //print_matrix(matrix);
        num_steps++;
    }

    double endOMP = omp_get_wtime();
    int end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Final matrix: \n");
    //print_matrix(matrix);
    printf("ELAPSED TIME: %lf sec\n", cpu_time_used);
    printf("ELAPSED TIME(OMP) %f sec\n", endOMP - startOMP);
    printf("TOTAL STEPS: %d\n", num_steps);
}