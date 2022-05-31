
#include <stdio.h>

#define Size = 10;
#define N = Size+1;

struct Vector2{
    int x;
    int y;
};

//Matrix definitions:
// The matrix are [N+1][N+1] cause the borders must be black
// 0 = black square
// 1 = white square
// 2 = G

int[N][N] copy_matrix(int[N][N] matrix){

    int[N][N] new_matrix;

    for(int i = 0; i < N; i++){
        for(int j = 0; i < N; j++){
            new_matrix[i][j] = matrix[i][j];
        }
    }

    return new_matrix;

}

//Lê matrix
void do_step(int[N][N] matrix, int[N][N] auxiliar){

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){

            if(matrix[i][j] == 1){



            }

        }
    }

}