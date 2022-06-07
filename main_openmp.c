#include <time.h>
#include "maze-resolution.c"
#include "pattern-1024.c"
#include "pattern-2048.c"

int main(void)
{

    int thread_id, n_threads, size;

    printf("NÚMERO DE THREADS:\n");
    scanf("%d", &n_threads);

    omp_set_num_threads(n_threads);

    int num_steps = 0;

    double start = omp_get_wtime();

    while (maze_sizeof_1024[N - 3][1] != Goal && num_steps < MAX_STEPS)
    {
        parallel_step(maze_sizeof_1024);
        num_steps++;
    }

    double end = omp_get_wtime();

    printf("TIME ELAPSED: %lfs", end - start);

}