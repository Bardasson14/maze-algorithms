#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "pattern_controller.c"
#include "mpi_utils.c"
#include "maze-resolution.c"

// TODO: liberar memória ao final

int main(int argc, char **argv)
{
    // Inicia MPI
    int initialized, finalized, rank, size;
    MPI_Initialized(&initialized);

    if (!initialized)
        MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double starttime, endtime;
    double total_time = 0;
    int num_steps = 0;
    int stopSteps = 0;
    int recebidoPorRank;
    int totalRows;

    int payloadBoundaries[2];
    int rankBoundaries[size][2]; // evita recálculo
    int nElem;
    int *dataChunk;

    // rank 0 apenas gerencia a comunicação
    if (rank == 0)
    {

        int matrix[N][N];
        copy_maze(maze_sizeof_8, matrix);
        printf("MATRIZ INICIAL: \n");
        print_matrix(matrix);
        num_steps = 0;

        starttime = MPI_Wtime();
        stopSteps = iterations_finished(matrix, num_steps);
        MPI_Bcast(&stopSteps, 1, MPI_INT, 0, MPI_COMM_WORLD);

        for(int i = 1; i < size; i++){

            calculatePayloadBoundaries(i, size - 1, payloadBoundaries);
            rankBoundaries[i][0] = payloadBoundaries[0];
            rankBoundaries[i][1] = payloadBoundaries[1];
            nElem = calculateTotalChunkSize(payloadBoundaries);
            MPI_Send(&nElem, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

        }

        while(!stopSteps){

            // Divide matrix atual para os workers
            for (int i = 1; i < size; i++)
            {
                dataChunk = fillChunk(matrix, dataChunk, rankBoundaries[i]);
                MPI_Send(dataChunk, nElem, MPI_INT, i, 0, MPI_COMM_WORLD);
                free(dataChunk);
            }
            
            // Recebe chunks já calculados de volta
            for (int i = 1; i < size; i++)
            {
                dataChunk = (int *)malloc(nElem * sizeof(int));
                payloadBoundaries[0] = rankBoundaries[i][0]; // start row
                payloadBoundaries[1] = rankBoundaries[i][1]; // end row
                nElem = calculateTotalChunkSize(payloadBoundaries);
                MPI_Recv(dataChunk, nElem, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                writeToMatrix(matrix, dataChunk, nElem, payloadBoundaries);// Junta chunks em matrix
                free(dataChunk);
            }

        
            num_steps++;
            stopSteps = iterations_finished(matrix, num_steps);
            MPI_Bcast(&stopSteps, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }

        endtime = MPI_Wtime();
        total_time = endtime - starttime;
        printf("ELAPSED TIME(MPI) %f sec\n", total_time);
        printf("TOTAL STEPS: %d\n", num_steps);
        printf("MATRIZ FINAL: \n");
        print_matrix(matrix);
    }
    
    else
    {

        MPI_Bcast(&stopSteps, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Recv(&nElem, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        dataChunk = (int *)malloc(nElem * sizeof(int));
        
        while (!stopSteps)
        {           
            MPI_Recv(dataChunk, nElem, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            mpiStep(dataChunk, rank, size);    
            MPI_Send(dataChunk, nElem, MPI_INT, 0, 0, MPI_COMM_WORLD); // devolver ao processo original
            MPI_Bcast(&stopSteps, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }

        free(dataChunk);

    }
    
    MPI_Finalize();
    return 0;
}