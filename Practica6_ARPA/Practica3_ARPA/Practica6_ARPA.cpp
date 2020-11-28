#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 10

int factorial(int n) {
	if (n < 0) return 0;
	else if (n > 1) return n * factorial(n - 1);
	return 1;
}

int main(int argc, char* argv[])
{	
	//Variables basicas
	int mirango, tamano, longitud;
	char nombre[32];
	int num, fact;

	MPI_Request envio, vuelta;
	MPI_Status status;

	//Funciones basicas
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
	MPI_Comm_size(MPI_COMM_WORLD, &tamano);
	MPI_Get_processor_name(nombre, &longitud);

	if (mirango == 0) {
		do {
			printf("Introduce un numero para hacer el factorial\n");
			printf("Terminar: 0\n");
			printf(":> ");
			fflush(stdout);
			scanf_s("%d", &num);
			if (num != 0) {


				// Envio de datos
				MPI_Isend(&num, 1, MPI_INT, 1, 7, MPI_COMM_WORLD, &envio);
				// Se tendrian que cerrar los send 

				// Recuperacion del factorial
				MPI_Irecv(&fact, 1, MPI_INT, 1, 8, MPI_COMM_WORLD, &vuelta);
				// espera la recepcion
				// Procesos intermedios
				MPI_Wait(&vuelta, &status);

				printf("Solucion: %i\n", fact);
			}
		} while (num != 0);
	}
	else if (mirango == 1) {
		MPI_Irecv(&num, 1, MPI_INT, 0, 7, MPI_COMM_WORLD, &envio);
		// espera la recepcion
		MPI_Wait(&envio, &status);

		printf("He recibido: %i\n", num);

		fact = factorial(num);
		wait();
		// devolucion de datos
		MPI_Isend(&fact, 1, MPI_INT, 0, 8, MPI_COMM_WORLD, &vuelta);

	}

	fflush(stdout);
	MPI_Finalize();
	return 0;
}