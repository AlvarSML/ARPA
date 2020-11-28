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

#define N 2

int main(int argc, char* argv[])
{
	int mirango, tamano;
	int longitud;
	char nombre[32];
	int msg = 111;
	char file[] = "Hijo.exe";

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
	MPI_Comm_size(MPI_COMM_WORLD, &tamano);
	MPI_Get_processor_name(nombre, &longitud);

	printf("Padre: %s\n",nombre);

	// Intercomm -> Intracomm
	MPI_Comm intercom; // Padre
	MPI_Comm intracom; // Padre - hijo

	MPI_Comm_spawn(file, MPI_ARGV_NULL, N, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercom, MPI_ERRCODES_IGNORE);

	//transformamos a ambos lados el intercom a intracom
	MPI_Intercomm_merge(intercom, 0, &intracom);

	// printf("Hola, soy el padre con mensaje %d.\n", msg);

	MPI_Bcast(&msg, 1, MPI_INT, 0, intracom);
	MPI_Finalize();
	return 0;
}
