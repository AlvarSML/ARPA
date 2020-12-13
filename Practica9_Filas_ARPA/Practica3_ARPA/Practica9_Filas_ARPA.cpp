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

//TAGS
#define PARAM 0 // Se indica que se manda un parametro
#define DATA 1 // Se indica que se manda un dato (fila)


int main(int argc, char* argv[])
{
	//Variables basicas
	int mirango, tamano, longitud;
	char nombre[128];
	int envio = 1, cierre = 0;


	// Gestion de errore
	int error_code;

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

	int filasProceso = ceil((float)dim / (tamano - 1));
	// printf("El array va a ser de %i x %i\n", dim, dim);

	// La unica matriz comun es la 2
	float** matriz2;
	matriz2 = (float**)malloc(dim * sizeof(float*));
	float* Mf2 = (float*)malloc(dim * dim * sizeof(float));
	for (int i = 0; i < dim; i++) {
		matriz2[i] = Mf2 + i * dim;
	}

	if (mirango == 0) {
		for (int i = 0; i < dim; i++) {
			matriz2[i] = Mf2 + i * dim;
		}

		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				matriz2[i][j] = (float)i + j;
			}
		}
	}

	MPI_Bcast(matriz2[0], dim * dim, MPI_FLOAT, 0, MPI_COMM_WORLD);

	if (mirango == 0) {

		float** matriz1, ** matrizRes;

		matriz1 = (float**)malloc(dim * sizeof(float*));
		matrizRes = (float**)malloc(dim * sizeof(float*));

		float* Mf1 = (float*)malloc(dim * dim * sizeof(float));
		float* MfRes = (float*)malloc(dim * dim * sizeof(float));

		for (int i = 0; i < dim; i++) {
			matriz1[i] = Mf1 + i * dim;
			matrizRes[i] = MfRes + i * dim;
		}

		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				matriz1[i][j] = 2.;
			}
		}

		
		int filasRestantes = dim;
		int filasMandar = filasProceso;
		int fila;

		printf("Matriz 1\n");
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				printf("%3.2f ", matriz1[i][j]);
			}
			printf("\n");
		}
		fflush(stdout);

		printf("Matriz 2\n");
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				printf("%3.2f ", matriz2[i][j]);
			}
			printf("\n");
		}
		fflush(stdout);


		// Se mandan filas a todos los procesos menos al 0
		for (int i = 1; i < tamano; i++) {
			// Numero de filas que se mandan			

			if (filasMandar > filasRestantes) {
				filasMandar = filasRestantes;
			}

			printf("[%i]Se van a mandar %i filas a %i\n", mirango, filasMandar, i);
			fflush(stdout);

			filasRestantes -= filasMandar;

			MPI_Send(&filasMandar, 1, MPI_INT, i, PARAM, MPI_COMM_WORLD);

			// Se mandan la filas
			for (int j = 0; j < filasMandar; j++) {
				// Que fila se va a enviar
				// NProceso * filasProceso + fila enviada
				fila = (i - 1) * filasProceso + j;

				error_code = MPI_Send(matriz1[fila], dim, MPI_FLOAT, i, DATA, MPI_COMM_WORLD);
				if (error_code != MPI_SUCCESS) {
					char error_string[BUFSIZ];
					int length_of_error_string;

					MPI_Error_string(error_code, error_string, &length_of_error_string);
					printf("!!%3d: %s\n", mirango, error_string);
					fflush(stdout);
				}
				else {
					printf("[%i]Se ha mandado la fila %i > %i\n", mirango, fila, i);
					fflush(stdout);
				}
			}
			printf("[%i]Todas las filas enviadas\n", mirango);
		}

		int filasRecibidas = 0;
		MPI_Status st;
		float* buff = (float*)malloc(dim*sizeof(float));
		// Se reciben las filas
		while (filasRecibidas != dim) {
			MPI_Recv(buff, dim, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
			printf("[%i] Recibida fila de %i \n",mirango ,st.MPI_TAG);

			matrizRes[st.MPI_TAG] = buff;
			filasRecibidas++;

		}

		printf("Matriz Resultado\n");
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				printf("%3.2f ", matrizRes[i][j]);
			}
			printf("\n");
		}
		fflush(stdout);


		// Recepcion de resultados
	}
	// El resto de los procesos, que van a hacer los calculos
	else {
		// Recepcion de las filas
		int nfilas;
		MPI_Status st;
		MPI_Recv(&nfilas, 1, MPI_INT, 0, PARAM, MPI_COMM_WORLD, &st);


		float** filas = (float**)malloc(nfilas * sizeof(float*));
		for (int i = 0; i < nfilas; i++) {
			filas[i] = (float*)malloc(dim * sizeof(float));
		}

		printf("[%i]Se van a recibir %i filas del %i \n", mirango, nfilas, st.MPI_SOURCE);
		fflush(stdout);

		for (int i = 0; i < nfilas; i++) {
			error_code = MPI_Recv(filas[i], dim, MPI_FLOAT, 0, DATA, MPI_COMM_WORLD, &st);
			if (error_code != MPI_SUCCESS) {
				char error_string[BUFSIZ];
				int length_of_error_string;

				MPI_Error_string(error_code, error_string, &length_of_error_string);
				printf("%3d: %s\n", mirango, error_string);
				fflush(stdout);
			}
			else {
				printf("[%i]Fila %i recibida con exito\n",mirango,(mirango * nfilas + i));
				fflush(stdout);
			}
		}


		float* filaRes = (float *)malloc(dim *sizeof(float));
		// Operaciones
		// Por cada fila
		for (int i = 0; i < nfilas; i++) {
			// Por cada valor
			for (int j = 0; j < dim; j++) {
				//printf("[%i]Fila calculada: ", mirango);
				// Operaciones sobre la matriz 2
				filaRes[j] = 0.;
				for (int k = 0; k < dim; k++) {
					filaRes[j] += (float)filas[i][k] * matriz2[k][j];
					//printf("[%f]",filas[i][j]);
				}
				//printf("\n");
				
			}

			// Una vez terminada una fila se manda directamente, ya que no requiere mucho tiempo
 
			int tag = (mirango-1) * filasProceso + i;
			MPI_Send(filaRes, dim, MPI_FLOAT, 0, tag , MPI_COMM_WORLD);
		}

	}

	printf("[%i] FINALIZADO ", mirango);

	fflush(stdout);
	MPI_Finalize();
	return 0;
}