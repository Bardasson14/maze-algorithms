#include "constants.h"
#include "patterns/pattern-64.c"
#include "patterns/pattern-512.c"
#include "patterns/pattern-1024.c"
#include "patterns/pattern-2048.c"

void copy_maze(int matrix[N][N], int copy_to[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            //printf("Copying matrix[%d][%d]\n", i, j);
            copy_to[i][j] = matrix[i][j];
        }
    }
}

void getPatternMaze(int maze[N][N]){
    if (Size == 64){
        copy_maze(maze_sizeof_64, maze);
    }
    if (Size == 512){
        copy_maze(maze_sizeof_512, maze);
    }
    if (Size == 1024){
        copy_maze(maze_sizeof_1024, maze);
    }
    if (Size == 2048){
        copy_maze(maze_sizeof_2048, maze);
    }
}