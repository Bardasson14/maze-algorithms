#include <time.h>
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
    int totalRows;
    int matrix[N][N];
    int *dataChunk;
    int payloadBoundaries[2][2];
    int nElem;

    

    // rank 0 apenas gerencia a comunicação

    if (rank == 0) {
        copy_maze(maze_sizeof_1024, matrix);
        num_steps = 0;

        for (int i=1; i<size;i++) {
            calculatePayloadBoundaries(i, size-1, payloadBoundaries);
            //printf("rank: %d\n", i);
            //printf("filling chunk...\n");
            //printf("chunk filled with data...\n");
            dataChunk = fillChunk(matrix, dataChunk, payloadBoundaries);
            nElem = sizeof(dataChunk)/sizeof(int);
            MPI_Send(&nElem, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&dataChunk[0], nElem, MPI_INT, i, 0, MPI_COMM_WORLD);
            free(dataChunk);
        }
    }
/*
    // Sincroniza relógio
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0){
        starttime = MPI_Wtime();
    }


        
    while (matrix[N - 2][1] != Goal && num_steps < MAX_STEPS && !stopSteps)
    {

        if (rank != 0) {
            MPI_Recv(&totalRows, 1, MPI_INT, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&dataChunk, totalRows*N, MPI_INT, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("step\n");
            mpi_step(dataChunk, rank, size-1); // AJUSTAR MPI_STEP
            //MPI_Barrier(MPI_COMM_WORLD); // TODOS DEVEM DAR STEP ANTES DE ESCREVEREM NA MATRIX, PARA EVITAR INCONSISTENCIA
            //writeToMatrix(matrix, dataChunk, rank, size); // TODO: efetivamente escrever na matriz
            // IDEIA: enviar para 0, que escreverá por ordem de Rank
        } 
        
        else {
            num_steps++;

            // Avisa aos trabalhadores que acabou while
            if (matrix[N - 2][1] == Goal || num_steps >= MAX_STEPS){
                stopSteps = 1;
            }

            MPI_Barrier(MPI_COMM_WORLD);
            MPI_Bcast(&stopSteps, 1, MPI_INT, 0, MPI_COMM_WORLD);
        }
    }

    // Sincroniza relógio
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0){
        //printf("Final matrix: \n");
        //print_matrix(matrix);
        endtime = MPI_Wtime();
        total_time = endtime - starttime;
        printf("ELAPSED TIME(MPI) %f sec\n", total_time);
        printf("TOTAL STEPS: %d\n", num_steps);
        // printf("\n%d", matrix[N - 2][1]);
    }
    */

    //free(matrix);

    MPI_Finalize();
    return 0;
}