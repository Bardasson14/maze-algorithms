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
        MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double starttime, endtime;
    double total_time = 0;
    int num_steps = 0;
    int stopSteps = 0;
    int recebidoPorRank;
    int totalRows;
    int matrix[N][N];
    int payloadBoundaries[2];
    int rankBoundaries[size][2]; // evita recálculo
    int nElem;
    int *dataChunk;

    // rank 0 apenas gerencia a comunicação
    if (rank == 0) {
        starttime = MPI_Wtime();
        copy_maze(maze_sizeof_64, matrix);
        //print_matrix(matrix);
        num_steps = 0;
    }

    while (matrix[N - 2][1] != Goal && num_steps < MAX_STEPS && !stopSteps) {

        if (rank == 0){
            // Divide matrix atual para os workers
            for (int i=1; i<size;i++) {
                calculatePayloadBoundaries(i, size-1, payloadBoundaries);
                dataChunk = fillChunk(matrix, dataChunk, payloadBoundaries);
                rankBoundaries[i][0] = payloadBoundaries[0];
                rankBoundaries[i][1] = payloadBoundaries[1];
                nElem = calculateTotalChunkSize(payloadBoundaries);
                //printf("nelem: %d\n", nElem);
                MPI_Send(&nElem, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(dataChunk, nElem, MPI_INT, i, 0, MPI_COMM_WORLD);
                free(dataChunk);
            }
        }

        if (rank != 0) {
            // Recebem chunks e realizam partialStep
            MPI_Recv(&nElem, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            dataChunk = (int*)malloc(nElem*sizeof(int));
            MPI_Recv(dataChunk, nElem, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("ChunkStep\n");
            mpiStep(dataChunk, rank, size); // mudar o STEP de lugar
            MPI_Send(dataChunk, nElem, MPI_INT, 0, 0, MPI_COMM_WORLD); // devolver ao processo original
        }

        if (rank == 0) {

            num_steps++;
            printf("num_steps: %d ", num_steps);

            // Avisa aos trabalhadores que acabou while
            if (matrix[N - 2][1] == Goal || num_steps >= MAX_STEPS){
                stopSteps = 1;
            }

            else {
                for (int i=1; i<size; i++) {
                    payloadBoundaries[0] = rankBoundaries[i][0]; // start row
                    payloadBoundaries[1] = rankBoundaries[i][1]; // end row
                    nElem = calculateTotalChunkSize(payloadBoundaries);
                    MPI_Recv(dataChunk, nElem, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    // Junta chunks em matrix
                    writeToMatrix(matrix, dataChunk, nElem, payloadBoundaries);
                }
            }
            //print_matrix(matrix);

            //MPI_Bcast(&stopSteps, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }

        // printf("TARGET: %d\n", matrix[N - 2][1] );
    }

    // MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0){
        //printf("Final matrix: \n");
        //print_matrix(matrix);
        endtime = MPI_Wtime();
        total_time = endtime - starttime;
        printf("ELAPSED TIME(MPI) %f sec\n", total_time);
        printf("TOTAL STEPS: %d\n", num_steps);
        printf("\n%d", matrix[N - 2][1]);
    }
    //free(matrix);

    MPI_Finalize();
    return 0;

}