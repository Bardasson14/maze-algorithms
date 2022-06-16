#include <math.h>
#include "constants.h"

int getIndex(int i, int j)
{
    return i * N + j;
}

// 0 1 2
// 3 4 5
// 6 7 8

// M = 3x3
// 3 threads
// 7/3 = 2
// 7%3 = 1

// OBS.: Manter operações aritméticas por meio de funções
void calculatePayloadBoundaries(int rank, int nWorkers, int payloadBoundariesMatrix[2])
{
    // FORMATO
    // [
    //  [i_inicial, j_inicial],
    //  [i_final, j_final],
    // ]

    int chunkSize, contiguousStart, startRow = 1, finalRow = Size, smallerChunk;
    div_t output;

    output = div(Size, nWorkers);
    chunkSize = output.quot;
    smallerChunk = output.rem;
    output = div((rank - 1) * Size, nWorkers); // desconsiderar processo 0, que só envia as mensagens

    // Size = 1024
    // nWorkers = 4
    // nElem = 1024*1024 = 1048576
    // rank 2
    // em termos de linha
    // partição 0: [0,   255]   (rank 1)
    // partição 1: [256, 511]   (rank 2)
    // partição 2: [512, 767]   (rank 3)
    // partição 3: [768, 1023]  (rank 4)

    if (rank != 1)
    {
        startRow = output.quot + 1;
    }

    if (smallerChunk != 0)
    {
        chunkSize = smallerChunk;
    }

    if (rank == nWorkers) // último processo com menos dados para tratar (Size%nThreads !=0)
    {
        finalRow = Size; // [1023]
    }
    else
    {
        finalRow = chunkSize + (rank - 1) * chunkSize;
    }

    payloadBoundariesMatrix[0] = startRow;
    payloadBoundariesMatrix[1] = finalRow;
}

int calculateTotalChunkSize(int payloadBoundaries[2]) {
    return (payloadBoundaries[1]-payloadBoundaries[0]+3)*N;
}

void writeToMatrix(int (*matrix)[N], int *dataChunk, int nElem, int payloadBoundaries[2]) // PREENCHER A MATRIZ ORIGINAL
{
    for(int chunkInd = N; chunkInd < (nElem-N); chunkInd++){
        int matrixRow = chunkInd / N;
        int matrixCol = chunkInd % N;
        matrix[matrixRow + payloadBoundaries[0] -1][matrixCol] = dataChunk[chunkInd];
    }
}

// preenche os chunks de dados com info da matriz
int *fillChunk(int (*matrix)[N], int *dataChunk, int payloadBoundaries[2])
{
    int chunkSize, continuousStart, upperLineIndex, lowerLineIndex, i_chunk;

    int index;
    int startPayloadRow = payloadBoundaries[0];
    int finalPayloadRow = payloadBoundaries[1];
    int totalRows = finalPayloadRow - startPayloadRow + 1 + 2;

    // incluindo linhas de vizinhança e paddings
    int totalChunkSizeBytes = totalRows * N * sizeof(int);
    dataChunk = (int *)malloc(totalChunkSizeBytes);

    for (int i = startPayloadRow - 1; i <= finalPayloadRow + 1; i++)
    {
        i_chunk = i - startPayloadRow + 1; //  +1 -> offset (primeira linha é de informação adicional)
        
        for (int j = 0; j < N; j++)
        {
            index = getIndex(i_chunk, j);
            dataChunk[index] = matrix[i][j];
        }
    }

    return dataChunk;
}