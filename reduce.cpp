#include <iostream>
#include <mpi.h>
#include <cstdio>
#include <cstdlib>


using namespace std;


double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int main(int argc, char * argv[]){
	MPI_Init(&argc, &argv);

	int n = atoi(argv[1]);
	int size, rank;

	MPI_Comm_rank(MPI_COMM_WORLD,  &rank);
	MPI_Comm_size(MPI_COMM_WORLD,  &size);

	double * a = (double * ) calloc(n, sizeof(double));
	double * b = (double * ) calloc(n, sizeof(double));

	double tick = MPI_Wtime();
	
	MPI_Reduce(a, b, n, MPI_DOUBLE,  MPI_SUM, 0, MPI_COMM_WORLD);

	double tack = MPI_Wtime();
	double time = tack - tick;

	if(rank == 0)
		printf("arrLen %d Nproc %d Time: %lf\n", n, size, time);

	MPI_Finalize();
	return 0;
}