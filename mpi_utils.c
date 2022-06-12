#include <math.h>
// #include <stdlib.h>
#include "constants.h"

// OBS.: Manter operações aritméticas por meio de funções
void calculatePayloadBoundaries(int rank, int size, int payloadBoundariesMatrix[2][2]) {
    // FORMATO
    // [
    //  [i_inicial, j_inicial],
    //  [i_final, j_final],
    //]

    int chunkSize, contiguousStart, startRow=0, startCol=0, finalRow, finalCol;
    div_t output;

    int nElem = (int)pow(N, 2);
    chunkSize = nElem/size;

    output = div((rank-1)*nElem, size);
    contiguousStart = output.quot; // desconsiderar processo 0, que só envia as mensagens

    if (rank != 1) {
        output = div(contiguousStart, N);
        startRow = output.quot;
        startCol = output.rem;
    } 

    output = div(nElem, size);

    if ((rank == (size-1)) && (output.quot)!=0) {
        output = div(nElem, size);
        chunkSize = output.rem; //nElem%size; // chunk final com tamanho menor, caso n_threads não seja divisor de N^2
        output = div(contiguousStart + chunkSize, N);
        finalRow = output.quot;
    } else {
        output = div(contiguousStart + chunkSize, N);
        finalRow = output.quot;
    }

    output = div(contiguousStart + chunkSize, N);
    finalCol = output.rem;

    payloadBoundariesMatrix[0][0] = startRow;
    payloadBoundariesMatrix[0][1] = startCol;
    payloadBoundariesMatrix[1][0] = finalRow;
    payloadBoundariesMatrix[1][1] = finalCol;

}

void writeToMatrix(int (*matrix)[N], int **dataChunk, int rank, int size) {
    int payloadBoundaries[2][2];
    int startRow, startCol, finalRow, finalCol;
    calculatePayloadBoundaries(rank, size, payloadBoundaries);

    startRow = payloadBoundaries[0][0];
    startCol = payloadBoundaries[0][1];
    finalRow = payloadBoundaries[1][0];
    finalCol = payloadBoundaries[1][1];

    // escrever na matriz
    // converter local para global

    /*
    for (int i=startRow; i<=finalRow; i++) {
        for (int j=0; j<=N; j++) {
            if !((i==startRow && j<startCol) || (i==finalRow && j>finalCol)) {
                // write

                // TODO: add verificações de pertencimento
                // ex.: i = start row e j < start col
                // ex.: i = final row e j > final col
            }
        }
    }
    */
   
}
 
int **mallocContiguousMatrix(int rows) {
    // garante que toda a matriz é composta de endereços seguidos em memória, facilitando o MPI_Send
    int *data = (int*)malloc(rows*N*sizeof(int));
    int **array= (int**)malloc(rows*sizeof(int*));
    for (int i=0; i<rows; i++){
        array[i] = &(data[N*i]);
        //printf("alocando %d\t - array[%d] = %d\n", i, i, array[i]);
    }

    return array;
}

int fillChunk(int (*matrix)[N], int **dataChunk, int rank, int size) {
    int chunkSize, continuousStart, totalRows, upperLineIndex, lowerLineIndex, i_chunk;

    int payloadBoundaries[2][2];
    
    calculatePayloadBoundaries(rank, size, payloadBoundaries);


    int startPayloadRow = payloadBoundaries[0][0];
    int finalPayloadRow = payloadBoundaries[1][0];
    totalRows = finalPayloadRow - startPayloadRow + 1 + 2; // incluindo linhas de vizinhança
    // printf("TOTAL_ROWS: %d\n", totalRows);
    // printf("finalPayloadRow: %d\n", finalPayloadRow);

    // preencher dados principais
    for (int i=startPayloadRow; i<= finalPayloadRow; i++) {
        i_chunk = i - startPayloadRow + 1; // +1 -> offset (primeira linha é de informação adicional)
        printf("i_matrix: %d\n", i);
        printf("i_chunk: %d\n", i_chunk);
        for (int j=0; j<N; j++) {
            dataChunk[i_chunk][j] = matrix[i][j];   
            //printf("matrix[%d][%d] = %d\n", i, j, matrix[i][j]);
            //printf("dataChunk[%d][%d] = matrix[%d][%d] = %d\n", i_chunk, j, i, j, dataChunk[i_chunk][j]);
        }
    }


    // preencher linhas adicionais
    printf("upper\n");
    if (startPayloadRow == 0) {
        for (int j=0; j<N; j++) dataChunk[0][j] = -1;
        printf("teste\n");
    }

    else {
        upperLineIndex = startPayloadRow - 1;
        for (int j=0; j<N; j++) dataChunk[0][j] = matrix[upperLineIndex][j];
    }

    printf("lower\n");
    if (finalPayloadRow == N-1) {
        for (int j=0; j<N; j++) dataChunk[totalRows-1][j] = -1;
    }

    else {
        lowerLineIndex = startPayloadRow - 1;
        for (int j=0; j<N; j++) dataChunk[totalRows-1][j] = matrix[lowerLineIndex][j];
    }



    return totalRows;
}