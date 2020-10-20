#include <mpi.h>
#include <stdio.h>

#define N 5

int main(int argc, char* argv[])
{
	int mirango, tamano;
	int longitud;
	char nombre[32];
	int dato;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
	MPI_Comm_size(MPI_COMM_WORLD, &tamano);
	MPI_Get_processor_name(nombre, &longitud);

	int arr[5];

	if (mirango == 0) {
		for (int i = 0; i < N; i++) {
			arr[i] = 7 * i;
		}
	}

	MPI_Bcast(arr, 5 * sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);

	printf("Proceso[%i]:",mirango);

	for (int i = 0; i < N; i++) {
		printf("[%i]",arr[i]);
	}
	printf("\n");

	fflush(stdout);
	MPI_Finalize();
	return 0;
}