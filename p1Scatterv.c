
#include "mpi.h"
#include <stdio.h>
#define SIZE 100

main(int argc, char *argv[])  {
	int numtasks, rank, source=0, dest, tag=1, i, stride;
    	float a[SIZE], acum, total;

        MPI_Status stat;

        for (i = 1; i <= SIZE; i++)
	        a[i-1] = i;


    	MPI_Init(&argc,&argv);
    	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	stride = SIZE/(numtasks);

        float b[stride];
        int disp[stride], scounts[stride];

    	for (i = 0; i < numtasks; i++) {
            	disp[i] = i*stride;
            	scounts[i] = stride;
    	}

    	MPI_Scatterv(a,scounts,disp,MPI_FLOAT,b,stride,MPI_FLOAT,0,MPI_COMM_WORLD);
    	acum = 0.0;
    	for (i = 0; i < stride; i++)
            	acum += b[i];
    	printf("Subtotal %lf en nodo %d\n", acum, rank);
    	MPI_Reduce(&acum,b,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
    	if (rank == 0) printf("TOTAL: %lf\n",b[0]);
    	MPI_Finalize();
}

