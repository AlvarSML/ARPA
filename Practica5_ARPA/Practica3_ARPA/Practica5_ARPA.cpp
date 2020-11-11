#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  

#define SIZE 10

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

	char file[] = "archivo1.txt";
	MPI_File filempi;

	int rc = MPI_File_open(MPI_COMM_WORLD, file, MPI_MODE_RDWR | MPI_MODE_CREATE, MPI_INFO_NULL, &filempi);

	if (rc) {
		// Si no se abre
		printf("Unable to open file \"file\"\n"); 
		fflush(stdout);
	}
	else {
		char options[] = "native";
		MPI_Status estado;
		int buff[] = { 48 + mirango };
		MPI_File_set_view(filempi, mirango *2, MPI_INT, MPI_INT,options, MPI_INFO_NULL);
		MPI_File_write_at(filempi, mirango *2, buff, 1, MPI_INT, &estado);

		MPI_File_close(&filempi);
	}

	fflush(stdout);
	MPI_Finalize();
	return 0;
}