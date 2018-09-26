#include <iostream>
#include <mpi.h>
#include <cstdio>
#include <cstdlib>


using namespace std;


#define N 10

int main(int argc, char * argv[]){
	MPI_Init(&argc, &argv);
	const int mSize = N;
	const int rCnt = 2 * N;
	int size, rank;
	double buf[mSize];
	double line[mSize];
	MPI_Request r[rCnt];
	
	for(int i = 0; i < mSize; i++){
		buf[i] = line[i] = i;
	}

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size); 
	
	double tick = MPI_Wtime();
	for(int i =0; i < mSize; i++){
		MPI_Isend(&buf[i], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &r[i]);
		MPI_Irecv(&line[i], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &r[mSize + i]);
	}
	MPI_Waitall(rCnt, r, MPI_STATUS_IGNORE);
	double tack = MPI_Wtime();
	double time = tack - tick;
	for(int i =0 ; i < mSize; i++){
		printf("%lf ", line[i]);
	}
	printf("Matrix size %d time %lf\n", mSize, time);
	MPI_Finalize();
	return 0;
}
