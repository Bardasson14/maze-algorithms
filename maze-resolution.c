#include <stdio.h>
#include <omp.h>
#include "constants.h"
#include "vector2.c"

// Matrix definitions:
//  The matrix are [N+2][N+2] cause the borders must be black
//  0 = black square
//  1 = white square
//  2 = G

int right_is(int matrix[N][N], Vector2 pos, int type)
{
    if (matrix[pos.x][pos.y + 1] == type)
        return 1;
    else
        return 0;
}

int left_is(int matrix[N][N], Vector2 pos, int type)
{
    if (matrix[pos.x][pos.y - 1] == type)
        return 1;
    else
        return 0;
}

int up_is(int matrix[N][N], Vector2 pos, int type)
{
    if (matrix[pos.x - 1][pos.y] == type)
        return 1;
    else
        return 0;
}

int down_is(int matrix[N][N], Vector2 pos, int type)
{
    if (matrix[pos.x + 1][pos.y] == type)
        return 1;
    else
        return 0;
}

int upper_right_is(int matrix[N][N], Vector2 pos, int type)
{
    if (matrix[pos.x - 1][pos.y + 1] == type)
        return 1;
    else
        return 0;
}

int down_right_is(int matrix[N][N], Vector2 pos, int type)
{
    if (matrix[pos.x + 1][pos.y + 1] == type)
        return 1;
    else
        return 0;
}

int upper_left_is(int matrix[N][N], Vector2 pos, int type)
{
    if (matrix[pos.x - 1][pos.y - 1] == type)
        return 1;
    else
        return 0;
}

int down_left_is(int matrix[N][N], Vector2 pos, int type)
{
    if (matrix[pos.x + 1][pos.y - 1] == type)
        return 1;
    else
        return 0;
}
int has_edge_of_type(int matrix[N][N], Vector2 pos, int neighbour_type)
{

    if (up_is(matrix, pos, neighbour_type))
        return 1;
    else if (right_is(matrix, pos, neighbour_type))
        return 1;
    else if (down_is(matrix, pos, neighbour_type))
        return 1;
    else if (left_is(matrix, pos, neighbour_type))
        return 1;
    else
        return 0;
}

void copy_matrix(int matrix[N][N], int copy_to[N][N])
{

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // printf("Copying matrix[%d][%d]\n", i, j);
            copy_to[i][j] = matrix[i][j];
        }
    }
}

void print_matrix(int matrix[N][N])
{

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d ", matrix[i][j]);
        }

        printf("\n");
    }

    printf("\n");
}

int evaluate_step(int matrix[N][N], Vector2 pos, int it_is)
{

    if (it_is == Black)
    {
        // Right, above and upper-right are White or Goal
        if ((right_is(matrix, pos, White) || right_is(matrix, pos, Goal)) && (up_is(matrix, pos, White) || up_is(matrix, pos, Goal)) && (upper_right_is(matrix, pos, White) || upper_right_is(matrix, pos, Goal)))
        {
            return White;
        }
    }

    else
    {
        // Right and above are black
        if (right_is(matrix, pos, Black) && up_is(matrix, pos, Black))
        {
            return Black;
        }

        else
        {
            if (has_edge_of_type(matrix, pos, Goal))
            {
                return Goal;
            }
        }
    }

    return it_is;
}

// Lê matrix
void do_step(int matrix[N][N])
{
    int write_into[N][N] = {0};

    for (int i = 1; i < N - 1; i++)
    {
        for (int j = 1; j < N - 1; j++)
        {
            Vector2 pos;
            pos.x = i;
            pos.y = j;
            write_into[i][j] = evaluate_step(matrix, pos, matrix[i][j]);
        }
    }

    copy_matrix(write_into, matrix);
}

// Lê matrix
void parallel_step(int matrix[N][N])
{
    int write_into[N][N] = {0};
    int thread_id, n_threads;

    #pragma omp parallel for schedule(dynamic) private(n_threads, thread_id) // collapse(2)

    for (int i = 1; i < N - 1; i++)
    {
        for (int j = 1; j < N - 1; j++)
        {

            thread_id = omp_get_thread_num();
            n_threads = omp_get_num_threads();

            // printf("\nCHECAGEM FEITA NA THREAD: %d - [%d][%d]\n", thread_id, i, j);
            // printf("TOTAL DE THREADS: %d\n\n", n_threads);

            Vector2 pos;
            pos.x = i;
            pos.y = j;

            write_into[i][j] = evaluate_step(matrix, pos, matrix[i][j]);
        }
    }
    copy_matrix(write_into, matrix);
}