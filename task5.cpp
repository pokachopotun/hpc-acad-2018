#include <iostream>
#include <mpi.h>
#include <cstdio>
#include <cstdlib>


using namespace std;


#define N 10

int main(int argc, char * argv[]){
	MPI_Init(&argc, &argv);
	const int mSize = N;
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(size >= mSize)
		size = mSize;
	int step = mSize / size + int(mSize % size != 0);
	int s = step * rank;
	int f = (rank + 1) * step;
	if(f > mSize)
		f = mSize;
	int lineCnt = f - s;
	if(lineCnt > 0)
	{
	double * cont = (double * ) calloc(lineCnt * mSize, sizeof(double));	
	double * buf = (double * ) calloc(lineCnt * mSize, sizeof(double));
	for(int line = 0; line < lineCnt; line++){
		for(int i =0 ; i < mSize; i++){
			int pos = line * mSize + i;
			cont[pos] = i;
			buf[pos] = i;
		}
	}
	double tick = MPI_Wtime();
	int rCnt = 2 * lineCnt * mSize;
	MPI_Request * r = (MPI_Request * ) calloc(rCnt, sizeof(MPI_Request));
	for(int line = 0; line < lineCnt; line++){
		for(int i =0 ; i < mSize; i++){
			int pos = line * mSize + i;
			int block = i / step;
			MPI_Isend(&buf[pos], 1, MPI_DOUBLE, block, line, MPI_COMM_WORLD, &r[pos]);
			MPI_Irecv(&cont[pos], 1, MPI_DOUBLE, block, i % step, MPI_COMM_WORLD, &r[rCnt/2 + pos]);
		}
	}
	MPI_Waitall(rCnt, r, MPI_STATUS_IGNORE);	
	double tack = MPI_Wtime();
	double time = tack - tick;
	/*if(rank == 0)
	{
		for(int line = 0; line < lineCnt; line++){
			for(int i =0 ; i < mSize; i++){
				int pos = line * mSize + i;
				printf("%lf ", cont[pos]);
			}
			printf("\n");
		}
	} */
	if(rank == 0){
		printf("Matrix size %dx%d Nodes %d Time %lf\n", mSize, mSize, size, time);
	}
	}
	MPI_Finalize();
	return 0;
}
