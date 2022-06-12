#include <math.h>
// #include <stdlib.h>
#include "constants.h"

int getIndex(int *arr, int i, int j) {
    return arr[i*N + j];
}

// OBS.: Manter operações aritméticas por meio de funções
void calculatePayloadBoundaries(int rank, int size, int payloadBoundariesMatrix[2][2])
{
    // FORMATO
    // [
    //  [i_inicial, j_inicial],
    //  [i_final, j_final],
    //]

    int chunkSize, contiguousStart, startRow = 0, startCol = 0, finalRow, finalCol;
    div_t output;

    int nElem = (int)pow(N, 2);
    chunkSize = nElem / size;

    output = div((rank - 1) * nElem, size); // desconsiderar processo 0, que só envia as mensagens
    contiguousStart = output.quot; 

    if (rank != 1)
    {
        output = div(contiguousStart, N);
        startRow = output.quot;
        startCol = output.rem;
    }

    output = div(nElem, size);

    if ((rank == (size - 1)) && (output.quot) != 0)
    {
        output = div(nElem, size);
        chunkSize = output.rem; // nElem%size; // chunk final com tamanho menor, caso n_threads não seja divisor de N^2
        output = div(contiguousStart + chunkSize, N);
        finalRow = output.quot;
    }
    else
    {
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

void writeToMatrix(int (*matrix)[N], int **dataChunk, int rank, int size)
{
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

/*
int **mallocContiguousMatrix(int rows) // TODO: liberar chunks
{
    // garante que toda a matriz é composta de endereços seguidos em memória, facilitando o MPI_Send
    printf("rows: %d\n", rows);
    int *mem = malloc(rows * N * sizeof(int));
    int **A = malloc(rows * sizeof(int *));
    A[0] = mem;

    for (int i = 1; i < rows; i++) {
        A[i] = A[i - 1] + N;
    }

    printf("\nalocação finalizada com sucesso\n");
    return mem;
}
*/


// preenche os chunks de dados com info da matriz
int *fillChunk(int (*matrix)[N], int *dataChunk, int rank, int size, int payloadBoundaries[2][2])
{
    int chunkSize, continuousStart, totalRows, upperLineIndex, lowerLineIndex, i_chunk;
    // TODO: add restrições colunas

    int index;
    int startPayloadRow = payloadBoundaries[0][0];
    int finalPayloadRow = payloadBoundaries[1][0];

    totalRows = finalPayloadRow - startPayloadRow + 1 + 2; // incluindo linhas de vizinhança

    long int totalChunkSizeBytes = totalRows * N * sizeof(int);
    dataChunk = malloc(totalChunkSizeBytes);

    /*
    for (int i=startPayloadRow; i< finalPayloadRow; i++) {
        i_chunk = i - startPayloadRow + 1; // +1 -> offset (primeira linha é de informação adicional)
        for (int j=0; j<N; j++) {
            index = getIndex(dataChunk, i_chunk, j);
            dataChunk[index] = matrix[i][j];
        }
    }*/

    for (int i=0;i<N;i++) {
        for (int j=0; j<N; j++) {
            printf("i: %d | j: %d\n", i, j);
        }
    }

    // preencher linhas adicionais
    printf("upper\n");
    if (startPayloadRow == 0)
    {
        for (int j = 0; j < N; j++) {
            index = getIndex(dataChunk, 0, j);
            dataChunk[index] = -1;
        }
    }

    else
    {
        upperLineIndex = startPayloadRow - 1;
        for (int j = 0; j < N; j++) {
            index = getIndex(dataChunk, upperLineIndex, j);
            dataChunk[index] = matrix[upperLineIndex][j];
        }
    }

    printf("lower\n");
    if (finalPayloadRow == N - 1)
    {
        for (int j = 0; j < N; j++) {
            index = getIndex(dataChunk, finalPayloadRow+1, j);
            dataChunk[index] = -1;
        }
    }

    else
    {
        lowerLineIndex = finalPayloadRow + 1;
        for (int j = 0; j < N; j++) {
            index = getIndex(dataChunk, finalPayloadRow+1, j);
            dataChunk[index] = matrix[lowerLineIndex][j];
        }
    }

    return dataChunk;
}