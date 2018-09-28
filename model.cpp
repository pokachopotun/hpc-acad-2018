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

	int nProc = size;

	double * a = (double * ) calloc(n, sizeof(double));
	double * b = (double * ) calloc(n, sizeof(double));
	double * c = (double * ) calloc(n, sizeof(double));

	for(int i =0 ;i < n; i++){
		a[i] = b[i] = 1;
		c[i] = 0;
	}

	double tick = MPI_Wtime();
	
	while(size > 1){
		int m = size / 2;
		int r = size - m;
		if(rank < m){
			MPI_Recv(c, n, MPI_DOUBLE, size - rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for(int i =0;i < n; i++)
				b[i] = c[i] + a[i];
		}else{
			if(rank < size)
				MPI_Send(b, n, MPI_DOUBLE, size - rank - 1, 0, MPI_COMM_WORLD);
		}
		size = m;
	}

	double tack = MPI_Wtime();
	double time = tack - tick;

	if(rank == 0)
		printf("Model arrLen %d Nproc %d Time: %lf\n", n, nProc, time);

	free(a);
	free(b);
	free(c);
	MPI_Finalize();
	return 0;
}
