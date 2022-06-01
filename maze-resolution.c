
#include <stdio.h>

#define Size 8
#define N Size+2


#define Black 0
#define White 1
#define Goal 2

typedef struct Vector2{
    int x;
    int y;
} Vector2;

//Matrix definitions:
// The matrix are [N+2][N+2] cause the borders must be black
// 0 = black square
// 1 = white square
// 2 = G

int right_is(int matrix[N][N], Vector2 pos, int type){
    if(matrix[pos.x][pos.y+1] == type)
        return 1;
    else
        return 0;
}

int left_is(int matrix[N][N], Vector2 pos, int type){
    if(matrix[pos.x][pos.y-1] == type)
        return 1;
    else
        return 0;
}

int up_is(int matrix[N][N], Vector2 pos, int type){
    if(matrix[pos.x-1][pos.y] == type)
        return 1;
    else
        return 0;
}

int down_is(int matrix[N][N], Vector2 pos, int type){
    if(matrix[pos.x+1][pos.y] == type)
        return 1;
    else
        return 0;
}

int upper_right_is(int matrix[N][N], Vector2 pos, int type){
    if(matrix[pos.x-1][pos.y+1] == type)
        return 1;
    else
        return 0;
}

int down_right_is(int matrix[N][N], Vector2 pos, int type){
    if(matrix[pos.x+1][pos.y+1] == type)
        return 1;
    else
        return 0;
}

int upper_left_is(int matrix[N][N], Vector2 pos, int type){
    if(matrix[pos.x-1][pos.y-1] == type)
        return 1;
    else
        return 0;
}

int down_left_is(int matrix[N][N], Vector2 pos, int type){
    if(matrix[pos.x+1][pos.y-1] == type)
        return 1;
    else
        return 0;
}
int has_edge_of_type(int matrix[N][N], Vector2 pos, int neighbour_type){

    if(up_is(matrix, pos, neighbour_type))
        return 1;
    else if(right_is(matrix, pos, neighbour_type))
        return 1;
    else if(down_is(matrix, pos, neighbour_type))
        return 1;
    else if(left_is(matrix, pos, neighbour_type))
        return 1;
    else
        return 0;
}

void copy_matrix(int matrix[N][N], int copy_to[N][N]){

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            //printf("Copying matrix[%d][%d]\n", i, j);
            copy_to[i][j] = matrix[i][j];
        }
    }


}

void print_matrix(int matrix[N][N]){

  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      printf("%d ", matrix[i][j]);
    }

    printf("\n");
    
  }

  printf("\n");
  
}

//Lê matrix
void do_step(int matrix[N][N]){

    int write_into[N][N] = {0};
  
    for(int i = 1; i < N-1; i++){
        for(int j = 1; j < N-1; j++){

            Vector2 pos;
            pos.x = i;
            pos.y = j;

            int it_is = matrix[i][j];
            int should_become = it_is;


            if(it_is == Black){

                //Right, above and upper-right are White or Goal
                if((right_is(matrix, pos, White) || right_is(matrix, pos, Goal)) && (up_is(matrix, pos, White) || up_is(matrix, pos, Goal)) && (upper_right_is(matrix, pos, White) || upper_right_is(matrix, pos, Goal)))
                    should_become = White;

            }
            else {

                //Right and above are black
                if(right_is(matrix, pos, Black) && up_is(matrix, pos, Black))
                    should_become = Black;
                else
                    if(has_edge_of_type(matrix, pos, Goal))
                        should_become = Goal;
                                    
                

            }

            write_into[i][j] = should_become;

        }
    }

  copy_matrix(write_into, matrix);


}




int main(int argc, char **argv) {

  // int matrix[N][N] = {{0, 0, 0, 0, 0},
  //                     {0, 1, 1, 2, 0},
  //                     {0, 1, 0, 0, 0},
  //                     {0, 1, 0, 0, 0},
  //                     {0, 0, 0, 0, 0}};

  // int matrix[N][N] = {{0, 0, 0, 0, 0},
  //                     {0, 1, 1, 1, 0},
  //                     {0, 1, 0, 1, 0},
  //                     {0, 1, 0, 2, 0},
  //                     {0, 0, 0, 0, 0}};

//   int matrix[N][N] = {{0, 0, 0, 0, 0},
//                       {0, 2, 1, 1, 0},
//                       {0, 1, 0, 1, 0},
//                       {0, 1, 1, 1, 0},
//                       {0, 0, 0, 0, 0}};
                      
  int matrix[N][N] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                      {0, 1, 1, 1, 1, 1, 0, 0, 0, 0},
                      {0, 1, 0, 0, 0, 1, 1, 0, 2, 0},
                      {0, 1, 0, 0, 0, 0, 1, 0, 1, 0},
                      {0, 1, 1, 0, 1, 0, 1, 1, 1, 0},
                      {0, 0, 1, 0, 1, 1, 1, 0, 0, 0},
                      {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                      {0, 0, 1, 0, 1, 1, 0, 0, 1, 0},
                      {0, 1, 1, 1, 1, 1, 0, 0, 1, 0},
                      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

  
  int num_steps = 0;
  printf("Initial matrix \n");
  print_matrix(matrix);

  while(matrix[N-2][1] != Goal && num_steps < 1000){

    do_step(matrix);
    printf("After %d° step \n", num_steps + 1);
    print_matrix(matrix);

    num_steps++;
      
  }

}