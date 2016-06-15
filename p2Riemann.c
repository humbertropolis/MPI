#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define XINICIAL 0
#define XFINAL 10
#define PASOS 1000
#define MASTER 0
#define TAG_INICIO 1
#define TAG_FIN 2
#define TAG_RESULT_PARCIAL 3

float calcularFuncion(float x);

int main (int argc, char *argv[])
{
      int rank, value, size, i,workers;  
      float resultado,incremento,x,resultParcial,xfinal;
      MPI_Status status; 

      incremento = ((float)XFINAL-(float)XINICIAL)/(float)PASOS;

      MPI_Init( &argc, &argv );

      MPI_Comm_rank( MPI_COMM_WORLD, &rank );
      MPI_Comm_size( MPI_COMM_WORLD, &size ); 
      resultParcial=0;
      workers=size-1;

      if (size<2){
    printf("No se puede procesar con %i nodos\n",size);
   return 0;
      }

      if (rank==MASTER){
    for (i=MASTER+1;i<size;i++){
      x=XINICIAL+(i-1)*XFINAL/(size-1);
      MPI_Send(&x, 1, MPI_FLOAT, i, TAG_INICIO, MPI_COMM_WORLD);
      xfinal=XINICIAL+i*XFINAL/(size-1);
      MPI_Send(&xfinal, 1, MPI_FLOAT, i, TAG_FIN, MPI_COMM_WORLD);
    }

    for (i=MASTER+1;i<size;i++){
      MPI_Recv(&resultParcial, 1, MPI_FLOAT, i, TAG_RESULT_PARCIAL,
                                            MPI_COMM_WORLD, &status);
      resultado +=resultParcial;
    }
        printf("El resultado de la suma de Riemann es %f \n",resultado);
}else{
     MPI_Recv(&x, 1, MPI_FLOAT, MASTER, TAG_INICIO, MPI_COMM_WORLD,&status);
     MPI_Recv(&xfinal, 1, MPI_FLOAT, MASTER, TAG_FIN, MPI_COMM_WORLD,
                                                          &status);
     while (x<xfinal)     {
        resultParcial += incremento * (calcularFuncion(x)+
                           calcularFuncion(x+incremento))/(float)2;
        x+=incremento;
     }
     MPI_Send(&resultParcial,1,MPI_FLOAT, MASTER, TAG_RESULT_PARCIAL
                                                 , MPI_COMM_WORLD);
}
  MPI_Finalize();
  return 0;
}

float calcularFuncion(float x){
  return x*x;
}

