#include <omp.h>
#include <mpi.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>

using namespace std;

#define VECTOR_SIZE 10000

#define NMAX 2048

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}


int main(int argc, char * argv[]){

	const int runTimes = 1000000;
	MPI_Init(&argc, &argv);

	int rank, size;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int a, b;
	double tick = MPI_Wtime();
	for(int i =0; i < runTimes; i++){
		if(rank == 0){
			MPI_Send(&a, 0, MPI_INT, 1, 0, MPI_STATUS_IGNORE, MPI_COMM_WORLD);
			MPI_Recv(&b, 0, MPI_INT, 1, 0, MPI_STATUS_IGNORE, MPI_COMM_WORLD);
		}
		else{
			MPI_Recv(&b, 0, MPI_INT, 0, 0, MPI_STATUS_IGNORE, MPI_COMM_WORLD);
			MPI_Send(&a, 0, MPI_INT, 0, 0, MPI_STATUS_IGNORE, MPI_COMM_WORLD);
		}
	}
	double tack = MPI_Wtime();
	double time = tack - tick;
	prinf("latency * %d %lf", runTimes, time)
	return 0;
}