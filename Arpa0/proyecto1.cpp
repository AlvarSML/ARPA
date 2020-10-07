#include <mpi.h>
#include <stdio.h>

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
	
	printf("Rango %i\n",mirango);
	fflush(stdout);

	if (mirango == 0) {		
		printf("Introduce un dato");
		fflush(stdout);
		scanf_s("%i", &dato);
		//scanf("%i", &dato);
		MPI_Send(&dato,1,MPI_INT,1,0,MPI_COMM_WORLD);
	}
	else {
		MPI_Status state;
		MPI_Recv(&dato, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &state);
		printf("Dato: %i\n", dato);
	}

	fflush(stdout);
	MPI_Finalize();
	return 0;
}