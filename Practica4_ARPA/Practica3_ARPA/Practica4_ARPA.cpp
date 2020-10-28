#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  

#define	N 2
#define M 2
#define DIMENSIONES 2

int main(int argc, char* argv[])
{
	int mirango, tamano, longitud;
	char nombre[32];
	int arr[N][M];
	int arrSum[N][M];
	int arrDest[N][M];
	int sum;

	int dims[] = { N,M };
	int period[] = { 1,0 }; //Son periodicas
	int procCoord[DIMENSIONES];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
	MPI_Comm_size(MPI_COMM_WORLD, &tamano);
	MPI_Get_processor_name(nombre, &longitud);

	printf("Proceso [%i]\n", mirango);
	fflush(stdout);

	if (mirango == 0) {
		/* initialize random seed: */
		srand(time(NULL));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M;j++) {
				arr[i][j] = rand() % 10;
				arrSum[i][j] = rand() % 10;
			}
		}

		printf("Array base:\n");
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				printf("[%i]",arr[i][j]);
			}
			printf("\n");
		}
		
		printf("Array a sumar:\n");
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				printf("[%i]", arrSum[i][j]);
			}
			printf("\n");
		}
	}

	MPI_Comm comunicadorCartesiano;
	
	// Para generar los procesos
	MPI_Cart_create(MPI_COMM_WORLD,DIMENSIONES,dims,period,1,&comunicadorCartesiano);	
	MPI_Bcast(arr, N*M, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(arrSum, N*M, MPI_INT, 0, MPI_COMM_WORLD);

	// Para recibir las coordenadas del proceso
	MPI_Cart_coords(comunicadorCartesiano,mirango,DIMENSIONES,procCoord);
	printf("Proceso[%i]:[%i][%i]\n", mirango, procCoord[0], procCoord[1]);
	// Suma de matrices
	sum = arr[procCoord[0]][procCoord[1]] + arrSum[procCoord[0]][procCoord[1]];

	
	MPI_Gather(&sum, 1, MPI_INT, arr, 1, MPI_INT, 0, MPI_COMM_WORLD);

	// Resultado
	if (mirango == 0) {
		printf("Array resultado:\n");
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				printf("[%i]", arr[i][j]);
			}
			printf("\n");
		}
	}

	
	fflush(stdout);
	MPI_Finalize();
	return 0;
}