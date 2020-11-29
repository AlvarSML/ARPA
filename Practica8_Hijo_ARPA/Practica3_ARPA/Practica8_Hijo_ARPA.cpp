/*
* ARQUITECTURAS PARALELAS
* 3º Grado en Ingeniería Informática
*
* PRACTICA 8: "Gestión dinámica de procesos" (Hijo)
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

#define NPROCESOS 5

int main(int argc, char* argv[])
{
	//Variables basicas
	int mirango, tamano, longitud,msg2;
	char nombre[32];

	int* errores;

	//Funciones basicas
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
	MPI_Comm_size(MPI_COMM_WORLD, &tamano);
	MPI_Get_processor_name(nombre, &longitud);

	printf("Soy el proceso hijo %i %s\n", mirango, nombre);
	fflush(stdout);

	// Comunicadores
	MPI_Comm intercom, intracom;
	MPI_Comm_get_parent(&intercom);
	MPI_Intercomm_merge(intercom, 1, &intracom);

	

	MPI_Bcast(&msg2,1,MPI_INT,0,intracom);
	printf("Mensaje del proceso padre: %i\n",msg2);
	fflush(stdout);

	int msg = 123;

	MPI_Bcast(&msg, 1, MPI_INT,0,MPI_COMM_WORLD);
	printf("Hola, soy el proceso hijo más pequeño[%i] :%d\n",mirango,msg);
	fflush(stdout);


	MPI_Finalize();
	return 0;
}
