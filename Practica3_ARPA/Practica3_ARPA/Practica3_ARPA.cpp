#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  

#define	N 5

int main(int argc, char* argv[])
{
	int mirango, tamano;
	int longitud;
	char nombre[32];
	int arrOrg[N];
	int arrOrg2[N];

	int num, num2, res;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mirango);
	MPI_Comm_size(MPI_COMM_WORLD, &tamano);
	MPI_Get_processor_name(nombre, &longitud);

	printf("Proceso [%i]\n", mirango);
	fflush(stdout);

	

	if (mirango == 0) {
		/* initialize random seed: */
		srand(time(NULL));
		for (int i = 0; i < N; i++) {
			arrOrg[i] = rand() % 10;
			arrOrg2[i] = rand() % 10;
		}

		for (int i = 0; i < N; i++) {
			printf("[%i]", arrOrg[i]);
		}
		printf("\n");
		for (int i = 0; i < N; i++) {
			printf("[%i]", arrOrg2[i]);
		}
		printf("\n");

	}	

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Scatter(arrOrg, 1, MPI_INT, &num,1,MPI_INT,0, MPI_COMM_WORLD);
	MPI_Scatter(arrOrg2, 1, MPI_INT, &num2, 1, MPI_INT, 0, MPI_COMM_WORLD);
	printf("Proceso[%i]: %i * %i = %i\n", mirango, num, num2, num*num2);
	
	num *= num2;	

	MPI_Reduce(&num, &res, 1, MPI_INT, MPI_SUM,0,MPI_COMM_WORLD);

	if (mirango == 0) {
		printf("Resultado [%i]\n",res);
		for (int i = 0; i < N; i++) {
			printf("[%i]", arrDest[i]);
		}
		printf("\n");
	}

	fflush(stdout);
	MPI_Finalize();
	return 0;
}