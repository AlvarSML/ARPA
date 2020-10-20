#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  

#define	FILAS 5
#define COLUMNAS 5

int main(int argc, char* argv[])
{
	int mirango, tamano;
	int longitud;
	char nombre[32];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
	MPI_Comm_size(MPI_COMM_WORLD, &tamano);
	MPI_Get_processor_name(nombre, &longitud);

	printf("Proceso [%i]\n", mirango);
	fflush(stdout);

	int orgArr[FILAS][COLUMNAS];

	/**
	int** orgArr = (int**)malloc(FILAS * sizeof(int*));
	// Si no hay memoria el puntero podria ser nulo
	for (int i = 0; i < FILAS; i++) {
		orgArr[i] = (int*)malloc(COLUMNAS * sizeof(int));
	}
	**/

	if (mirango == 0) {
		/* initialize random seed: */
		srand(time(NULL));
		for (int i = 0; i < FILAS; i++) {
			for (int j = 0; j < COLUMNAS; j++) {
				orgArr[i][j] = (int)rand();
				printf("[%i]", orgArr[i][j]);
			}
			printf("\n");
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);// sincroniza los procesadores
	MPI_Bcast(orgArr, (FILAS * COLUMNAS) * sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);



	for (int i = 0; i < FILAS; i++) {
		for (int j = 0; j < COLUMNAS; j++) {
			printf("%i ", orgArr[i][j]);
		}
		printf("\n");
	}



	fflush(stdout);
	MPI_Finalize();
	return 0;
}