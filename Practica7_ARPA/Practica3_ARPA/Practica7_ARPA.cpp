/*
* ARQUITECTURAS PARALELAS
* 3º Grado en Ingeniería Informática
*
* PRACTICA 7: "Tipos de datos derivados"
*
* AUTOR: Alberto Diez Busto
* Rubén Herrero Vicario
* Alvar San Martín Liendo
*/
///////////////////////////////////////////////////////////////////////////

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>

#define N 5

int main(int argc, char* argv[])
{
	//Variables basicas
	int mirango, tamano, longitud;
	char nombre[32];

	// Matrices
	int matrx[N][N];

	//Funciones basicas
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
	MPI_Comm_size(MPI_COMM_WORLD, &tamano);
	MPI_Get_processor_name(nombre, &longitud);

	//Matriz superior
		// 1 1 1 1  4 0
		// x 1 1 1  3 5 N+1
		// x x 1 1  2 10
		// x x x 1  1 15
		//Matriz inferior
		// 1 x x x  1 0 
		// 1 1 x x  2 3 4 N
		// 1 1 1 x  3 2 8
		// 1 1 1 1  4 1 12

	int longitudesSup[N];
	int longitudesInf[N];
	for (int i = 0; i < N; i++) {
		longitudesSup[i] = N - i;
		longitudesInf[i] = i + 1;
	}

	int despSup[N];
	int despInf[N];
	for (int i = 0; i < N; i++) {
		despSup[i] = ((i == 0) ? 0 : despSup[i - 1] + N + 1);
		despInf[i] = ((i == 0) ? 0 : despInf[i - 1] + N);

		if (mirango == 0)
			printf("%i{%i}", i, despSup[i]);
	}
	if (mirango == 0)
		printf("\n");


	MPI_Datatype vector_tipos[N];
	for (int i = 0; i < N; i++) {
		vector_tipos[i] = MPI_INT;
	}


	MPI_Datatype tipoSup, tipoInf;
	MPI_Type_indexed(N, longitudesSup, despSup, MPI_INT, &tipoSup);
	MPI_Type_indexed(N, longitudesInf, despInf, MPI_INT, &tipoInf);
	MPI_Type_commit(&tipoSup);
	MPI_Type_commit(&tipoInf);

	if (mirango == 0) {

		// Relleno de la 1a matriz
		srand(time(NULL));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				matrx[i][j] = rand() % 10;
				printf("[%i]", matrx[i][j]);
			}
			printf("\n");
		}
		printf("\n");


		MPI_Send(matrx, 1, tipoSup, 1, 0, MPI_COMM_WORLD);
		MPI_Send(matrx, 1, tipoInf, 2, 0, MPI_COMM_WORLD);

	}
	else if (mirango == 1) {

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				matrx[i][j] = NULL;
			}
		}
		MPI_Status state;
		MPI_Recv(matrx, 1, tipoSup, 0, 0, MPI_COMM_WORLD, &state);
		printf(">Matriz Superior\n");
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				printf("[%i]", matrx[i][j]);
			}
			printf("\n");
		}
		printf("\n");

	}
	else if (mirango == 2) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				matrx[i][j] = NULL;
			}
		}
		MPI_Status state;
		MPI_Recv(matrx, 1, tipoInf, 0, 0, MPI_COMM_WORLD, &state);
		printf(">Matriz Inferior\n");
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				printf("[%i]", matrx[i][j]);
			}
			printf("\n");
		}
		printf("\n");
	}



	fflush(stdout);
	MPI_Finalize();
	return 0;
}