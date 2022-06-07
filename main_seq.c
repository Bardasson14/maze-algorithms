#include <time.h>
#include "maze-resolution.c"
#include "pattern-1024.c"
#include "pattern-2048.c"

int main(int argc, char **argv)
{

  int num_steps = 0;

  double start = omp_get_wtime();

  while (maze_sizeof_1024[N - 3][1] != Goal && num_steps < MAX_STEPS)
  {
    do_step(maze_sizeof_1024);
    num_steps++;
  }

  double end = omp_get_wtime();
  double cpu_time_used = end - start;

  printf("ELAPSED TIME: %lf s\n", cpu_time_used);
}