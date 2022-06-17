#include <time.h>
#include <stdlib.h>
#include "pattern_controller.c"
#include "maze-resolution.c"
#include "mpi_utils.c"

int main(int argc, char **argv)
{

  int (*matrix)[N] = malloc(sizeof(int[N][N]));
  copy_maze(maze_sizeof_8, matrix);

  //getPatternMaze(matrix); - TODO: ajustar

  int num_steps = 0;
  printf("Maze with size: %d\n", Size);
  printf("MATRIZ INICIAL:\n");
  print_matrix(matrix);

  //printf("Initial matrix \n");
  //print_matrix(matrix);

  int start = clock();

  while (matrix[N - 2][1] != Goal && num_steps < MAX_STEPS)
  {
    seqStep(matrix);
    // printf("After %d° step \n", num_steps + 1);
    // print_matrix(matrix);
    num_steps++;
  }

  int end = clock();
  double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

  //printf("Final matrix: \n");
  //print_matrix(matrix);


  printf("ELAPSED TIME: %lf s\n", cpu_time_used);
  printf("TOTAL STEPS: %d\n", num_steps);

  printf("MATRIZ FINAL:\n");
  print_matrix(matrix);

}