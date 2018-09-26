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

	MPI_Init(&argc, &argv);
	double tick = MPI_Wtime();
	int rank, size, used, part, last;
	double buf[NMAX];
	double localSum = 0;
	double globalSum = 0;

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(size >= VECTOR_SIZE){
		part = 1;
		last = 1;
		used = VECTOR_SIZE;
	}else{
		part = VECTOR_SIZE / size;
		last = VECTOR_SIZE % size;
		if(last == 0){
			last = part;
		}
		used = size;
	}
	double * a = (double * ) calloc(last, sizeof(double));
	double * b = (double * ) calloc(last, sizeof(double));

	MPI_Request r[NMAX];
	if(rank < used){
		#pragma omp parallel for
		for(int i = 0; i < last; i++){
			a[i] = fRand(0.001, 1.0);
			b[i] = fRand(0.001, 1.0);
		}
		#pragma omp parallel for reduction(+ : localSum)
		for(int i = 0; i < last; i++){
			localSum += a[i] * b[i];
		}
		// print("localSum %d\n", localSum);
		MPI_Isend(&localSum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &r[rank]);
	}
	if(rank == 0){
		for(int i=0; i < used; i++){
			MPI_Irecv(&buf[i], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &r[i], MPI_STATUS_IGNORE);
		}
		MPI_Waitall(used, r, MPI_STATUS_IGNORE);
		#pragma omp parallel for reduction(+ : globalSum)
		for(int i= 0; i < used; i++){
			globalSum += buf[i];
		}
		print("globalSum %d\n", globalSum);	
	}

	free(a);
	free(b);
	double tack = MPI_Wtime();
	double time = tack - tick;
	print("VECTOR_SIZE %d nodes %d used %d time %lf\n", VECTOR_SIZE, size, used, time);
	MPI_Finalize();

	return 0;
}