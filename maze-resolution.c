#include <stdio.h>
#include <omp.h>
#include "mpi.h"
#include "constants.h"
#include "vector2.c"

// Matrix definitions:
//  The matrix are [N+2][N+2] cause the borders must be black
//  0 = black square
//  1 = white square
//  2 = G

int iterations_finished(int matrix[N][N], int num_steps) {
    if (matrix[N - 2][1] == Goal || num_steps >= MAX_STEPS) {
        return 1;
    }

    return 0;
}

int right_is(int (*matrix)[N], int *dataChunk, Vector2 pos, int type)
{
    if (dataChunk != NULL) {
        if (dataChunk[getIndex(pos.x, pos.y+1)] == type) {
            return 1;
        }

        else {
            return 0;
        }
    }

    else {
        if (matrix[pos.x][pos.y + 1] == type)
            return 1;
        else
            return 0;
    }
}

int left_is(int (*matrix)[N], int *dataChunk, Vector2 pos, int type)
{
    if (dataChunk != NULL) {
        if (dataChunk[getIndex(pos.x, pos.y-1)] == type) {
            return 1;
        }

        else {
            return 0;
        }
    }

    else {
        if (matrix[pos.x][pos.y - 1] == type)
            return 1;
        else
            return 0;
    }
}

int up_is(int (*matrix)[N], int *dataChunk, Vector2 pos, int type)
{
    if (dataChunk != NULL) {
        if (dataChunk[getIndex(pos.x-1, pos.y)] == type) {
            return 1;
        }

        else {
            return 0;
        }
    }

    else {
        if (matrix[pos.x - 1][pos.y] == type)
            return 1;
        else
            return 0;
    }
}

int down_is(int (*matrix)[N], int *dataChunk, Vector2 pos, int type)
{
    if (dataChunk != NULL) {
        if (dataChunk[getIndex(pos.x+1, pos.y)] == type) {
            return 1;
        }
        else {
            return 0;
        }
    }

    else {
        if (matrix[pos.x + 1][pos.y] == type)
            return 1;
        else
            return 0;
    }
}

int upper_right_is(int (*matrix)[N], int *dataChunk, Vector2 pos, int type)
{
    if (dataChunk != NULL) {
        if (dataChunk[getIndex(pos.x-1, pos.y+1)] == type) {
            return 1;
        }
        else {
            return 0;
        }
    }

    else {
        if (matrix[pos.x-1][pos.y+1] == type)
            return 1;
        else
            return 0;
    }
}

int down_right_is(int (*matrix)[N], int *dataChunk, Vector2 pos, int type)
{
    if (dataChunk != NULL) {
        if (dataChunk[getIndex(pos.x+1, pos.y+1)] == type) {
            return 1;
        }
        else {
            return 0;
        }
    }

    else {
        if (matrix[pos.x + 1][pos.y + 1] == type)
            return 1;
        else
            return 0;
    }
}

int upper_left_is(int (*matrix)[N], int *dataChunk, Vector2 pos, int type)
{
    if (dataChunk != NULL) {
        if (dataChunk[getIndex(pos.x-1, pos.y-1)] == type) {
            return 1;
        }
        else {
            return 0;
        }
    }

    else {
        if (matrix[pos.x - 1][pos.y - 1] == type)
            return 1;
        else
            return 0;
    }
}

int down_left_is(int (*matrix)[N], int *dataChunk, Vector2 pos, int type)
{
     if (dataChunk != NULL) {
        if (dataChunk[getIndex(pos.x+1, pos.y-1)] == type) {
            return 1;
        }
        else {
            return 0;
        }
    }

    else {
        if (matrix[pos.x + 1][pos.y - 1] == type)
            return 1;
        else
            return 0;
    }
}
int has_edge_of_type(int (*matrix)[N], int *dataChunk, Vector2 pos, int neighbour_type)
{

    if (up_is(matrix, dataChunk, pos, neighbour_type))
        return 1;
    else if (right_is(matrix, dataChunk, pos, neighbour_type))
        return 1;
    else if (down_is(matrix, dataChunk, pos, neighbour_type))
        return 1;
    else if (left_is(matrix, dataChunk, pos, neighbour_type))
        return 1;
    else
        return 0;
}

void copy_matrix(int (*matrix)[N], int copy_to[N][N])
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

void copyChunk(int **write_into, int *dataChunk, int rows)
{
    int index;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < N; j++)
        {
            index = getIndex(i,j);
            dataChunk[index] = write_into[i][j];
        }
    }
}

void make_zeros(int (*matrix)[N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix[i][j] = 0;
        }
    }
}

void print_matrix(int (*matrix)[N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d", matrix[i][j]);
        }

        printf("\n");
    }

    printf("\n");
}


int evaluateStepMPI(int *dataChunk, Vector2 pos, int it_is)
{

    if (it_is == Black)
    {
        // Right, above and upper-right are White or Goal
        if ((right_is(NULL, dataChunk, pos, White) || right_is(NULL, dataChunk, pos, Goal)) && (up_is(NULL, dataChunk, pos, White) || up_is(NULL, dataChunk, pos, Goal)) && (upper_right_is(NULL, dataChunk, pos, White) || upper_right_is(NULL, dataChunk, pos, Goal)))
        {
            return White;
        }
    }

    else
    {
        // Right and above are black
        if (right_is(NULL, dataChunk, pos, Black) && up_is(NULL, dataChunk, pos, Black))
        {
            return Black;
        }

        else
        {
            if (has_edge_of_type(NULL, dataChunk, pos, Goal))
            {
                return Goal;
            }
        }
    }
    return it_is;
}

int evaluateStep(int (*matrix)[N], Vector2 pos, int it_is)
{

    if (it_is == Black)
    {
        // Right, above and upper-right are White or Goal
        if ((right_is(matrix, NULL, pos, White) || right_is(matrix, NULL, pos, Goal)) && (up_is(matrix, NULL, pos, White) || up_is(matrix, NULL, pos, Goal)) && (upper_right_is(matrix, NULL, pos, White) || upper_right_is(matrix, NULL, pos, Goal)))
        {
            return White;
        }
    }

    else
    {
        // Right and above are black
        if (right_is(matrix, NULL, pos, Black) && up_is(matrix, NULL, pos, Black))
        {
            return Black;
        }

        else
        {
            if (has_edge_of_type(matrix, NULL, pos, Goal))
            {
                return Goal;
            }
        }
    }

    return it_is;
}

// Lê matrix sequencial

void seqStep(int (*matrix)[N])
{
    int write_into[N][N] = {0};

    for (int i = 1; i < N - 1; i++)
    {
        for (int j = 1; j < N - 1; j++)
        {
            Vector2 pos;
            pos.x = i;
            pos.y = j;
            write_into[i][j] = evaluateStep(matrix, pos, matrix[i][j]);
        }
    }

    copy_matrix(write_into, matrix);
}

// Lê matrix OpenMP
void ompStep(int (*matrix)[N])
{
    int write_into[N][N] = {0};
    int thread_id, n_threads;
    #pragma omp parallel shared(matrix, write_into) // collapse(2)
    {
        int i = 1, j = 1;
        #pragma omp for schedule(auto) private(i, j)
        for (i = 1; i < N - 1; i++)
        {
            for (j = 1; j < N - 1; j++)
            {
                Vector2 pos;
                pos.x = i;
                pos.y = j;

                write_into[i][j] = evaluateStep(matrix, pos, matrix[i][j]);
            }
        }
        copy_matrix(write_into, matrix);
    }
}

// Lê matrix MPI
// O mpiStep tem que ser convertido para lidar apenas com uma fração da Matriz
// Deve juntar todos os chunks de dados, considerando só a carga útil

void mpiStep(int *dataChunk, int rank, int size)
{
    int payloadBoundaries[2];
    calculatePayloadBoundaries(rank, size-1, payloadBoundaries);
    int rows = payloadBoundaries[1]-payloadBoundaries[0]+3;
    int cols = N;


    int **write_into = (int**)malloc(rows*sizeof(int*));
    write_into[0] = (int *)malloc(N * sizeof(int));
    write_into[rows-1] = (int *)malloc(N * sizeof(int));

    for(int i = 1; i < rows-1; i++) {
        write_into[i] = (int *)malloc(N * sizeof(int));
        for (int j=0; j < N; j++) {
            Vector2 pos;
            pos.x = i;
            pos.y = j;
            write_into[i][j] = evaluateStepMPI(dataChunk, pos, dataChunk[getIndex(i, j)]);
            
        }
    } 

    copyChunk(write_into, dataChunk, rows);

    for(int i = 1; i < rows-1; i++){
        free(write_into[i]);
    }
    free(write_into);

}