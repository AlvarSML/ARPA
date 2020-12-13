/*
* ARQUITECTURAS PARALELAS
* 3� Grado en Ingenier�a Inform�tica
*
* PRACTICA 7: "Tipos de datos derivados"
*
* AUTOR: Alberto Diez Busto
* Rub�n Herrero Vicario
* Alvar San Mart�n Liendo
*/
///////////////////////////////////////////////////////////////////////////

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

#define N 5

int main(int argc, char* argv[])
{
	//Variables basicas
	int mirango, tamano, longitud;
	char nombre[32];

	//Funciones basicas
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
	MPI_Comm_size(MPI_COMM_WORLD, &tamano);
	MPI_Get_processor_name(nombre, &longitud);

	//Practica
	int dim;

	if (mirango == 0) {		
		printf("Introduce la dimension de la matriz cuadrada:\n");
		fflush(stdout);
		scanf_s("%i", &dim);
	}
	//MPI_Barrier(MPI_COMM_WORLD);
	//Envio de las dimensiones del array
	MPI_Bcast(&dim, 1, MPI_INT, 0, MPI_COMM_WORLD);

	printf("El array va a ser de %i x %i\n",dim,dim);

	float** matriz1, ** matriz2, **matrizRes, **matrizBuff;

	matriz1 = (float**)malloc(dim * sizeof(float*));
	matriz2 = (float**)malloc(dim * sizeof(float*));
	matrizRes = (float**)malloc(dim * sizeof(float*));
	matrizBuff = (float**)malloc(dim * sizeof(float*));

	float* Mf1 = (float*)malloc(dim * dim * sizeof(float));
	float* Mf2 = (float*)malloc(dim * dim * sizeof(float));
	float* MfRes = (float*)malloc(dim * dim * sizeof(float));
	float* MfBuff = (float*)malloc(dim * dim * sizeof(float));

	for (int i = 0; i < dim; i++)	{
		matriz1[i] = Mf1 + i * dim;
		matriz2[i] = Mf2 + i * dim;
		matrizRes[i] = MfRes + i * dim;
		matrizBuff[i] = MfBuff + i * dim;
	}

	if (mirango == 0) {
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				matriz1[i][j] = 2.;
				matriz2[i][j] = (float)i+j;
				matrizRes[i][j] = 0;
 			}
		}
	}

	// Envio de matrices
	// La 1 se tendra que mandar solo por filas
	MPI_Bcast(matriz1[0], dim * dim, MPI_FLOAT, 0, MPI_COMM_WORLD);
	MPI_Bcast(matriz2[0], dim * dim, MPI_FLOAT, 0, MPI_COMM_WORLD);

	// Calculo de numeros / proceso
	int numsProceso = ceil( (float)(dim * dim)/ (float)tamano );

	// Calculo del valor de inicio
	int id = mirango * numsProceso;

	// Paso a coordenadas
	int fila = id / dim;
	int columna = id % dim;

	// Acumulador de los valores de la fila
	float acc = 0;
	// Por cada posicion de la matriz original asignado
	for (int i = 0; i < numsProceso; i++) {
		// Por cada valor de la fila
		for (int j = 0; j < dim; j++) {
			acc += matriz1[fila][j] * matriz2[j][columna];
		}


		// No se porque es asi pero funciona
		matrizRes[fila][columna] = acc;

		if ((columna + 1) == dim) {
			columna = 0;
			fila++;
		}
		else {
			columna++;
		}

		acc = 0;
	}

	printf("Matriz Resultado\n");
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			printf("%f ", matrizRes[i][j]);
		}
		printf("\n");
	}
	fflush(stdout);

	MPI_Barrier(MPI_COMM_WORLD);
	if (mirango != 0) {
		//Cambiar por isend		
		MPI_Send(matrizRes,dim*dim,MPI_FLOAT,0,1,MPI_COMM_WORLD);
	}
	
	// Comprobacion de recepcion
	if (mirango == 0) {

		// ESTA MAL
		for (int i = 1; i < tamano; i++) {
			MPI_Status st;
			MPI_Recv(matrizBuff,dim*dim, MPI_FLOAT,MPI_ANY_SOURCE,1, MPI_COMM_WORLD, &st);

			printf("Matriz Buffer (%i) \n",st.MPI_SOURCE);
			printf("Errores %i \n", st.MPI_ERROR);
			/*
			for (int i = 0; i < dim; i++)
			{
				for (int j = 0; j < dim; j++)
				{
					printf("%f ", matrizBuff[i][j]);
				}
				printf("\n");
			}
			fflush(stdout);

			// Calculo del valor de inicio
			id = i * numsProceso;

			// Paso a coordenadas
			fila = id / dim;
			columna = id % dim;

			for (int i = 0; i < dim;  i++) {

				matrizRes[fila][columna] = matrizBuff[fila][columna];

				if ((columna + 1) == dim) {
					columna = 0;
					fila++;
				}
				else {
					columna++;
				}
			}
			*/
		}

		printf("Nums Proceso: %i\n",numsProceso);
		fflush(stdout);
		printf("Matriz 1\n");
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				printf("%f ", matriz1[i][j]);
			}
			printf("\n");
		}
		fflush(stdout);

		printf("Matriz 2\n");
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				printf("%f ", matriz2[i][j]);
			}
			printf("\n");
		}
		fflush(stdout);

		printf("Matriz Resultado\n");
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				printf("%f ", matrizRes[i][j]);
			}
			printf("\n");
		}
		fflush(stdout);
	}

	
	

	fflush(stdout);
	MPI_Finalize();
	return 0;
}