#include "constants.h"
#include <stdio.h>
#include "./patterns/pattern-8.c"
#include "./patterns/pattern-64.c"
#include "./patterns/pattern-128.c"
#include "./patterns/pattern-256.c"
#include "./patterns/pattern-512.c"
#include "./patterns/pattern-1024.c"
#include "./patterns/pattern-2048.c"

void copy_maze(int (*matrix)[N], int (*copy_to)[N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            copy_to[i][j] = matrix[i][j];
        }
    }
}