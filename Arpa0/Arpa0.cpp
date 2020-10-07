#include <mpi.h>
#include <stdio.h>h

int main(int argc, char* argv[])
{
	int mirango, tamano;
	int longitud;
	char nombre[32];
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
	MPI_Comm_size(MPI_COMM_WORLD, &tamano);
	MPI_Get_processor_name(nombre, &longitud);
	printf("[Maquina %s]> Proceso %d de %d: Hola Mundo!\n", nombre, mirango, tamano);
	fflush(stdout);
	MPI_Finalize();
	return 0;
}