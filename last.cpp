#include <iostream>
#include <mpi.h>
#include <cstdio>
#include <cstdlib>


using namespace std;


int main(int argc, char * argv[]){
	MPI_Init(&argc, &argv);
	int mSize = atoi(argv[1]);
	int size, rank;

	MPI_Comm_rank(MPI_COMM_WORLD,  &rank);
	MPI_Comm_size(MPI_COMM_WORLD,  &size);

	
	double * a, *b, *buf, *c;

	int step = mSize / size + int(mSize % size  > 0);
	int s = step * rank;
	int f = step * ( rank + 1);
	if(f > mSize)
		f = mSize;
	int used = mSize / step + int(mSize % step != 0);
	

	if(s < mSize){

		int lineCnt = f - s;
		int localSize = mSize * lineCnt;
		// printf("localSize: %d\n", localSize);
		int rCnt = 2 * used;
		MPI_Request * r = (MPI_Request * )calloc( rCnt, sizeof(MPI_Request));
		a = (double * ) calloc(localSize, sizeof(double));
		b = (double * ) calloc(localSize, sizeof(double));
		buf = (double * ) calloc(step * mSize, sizeof(double));
		c = (double * ) calloc(localSize, sizeof(double));

		for(int i = s; i < f; i++){
			for(int j = 0; j < mSize; j++){
				int pos = mSize * (i - s) + j;
				if(i == j){
					a[pos] = 1.0;// double(mSize - 2) / mSize;
					c[pos] = 0.0;
				}else{
					a[pos] = 0.0; //double(-2.0)/mSize;
					c[pos] = 0.0;
				}
			}
		}

		for(int i = s; i < f; i++){
			for(int j = 0; j < mSize; j++){
				int pos = mSize * (i - s) + j;
				if(i == j){
					b[pos] = 1.0; //double(mSize - 2) / mSize;
				}else{
					b[pos] = 0.0;//double(-2.0)/mSize;
				}
			}
		}
////////////////////////////////////////////

		double tick = MPI_Wtime();	
		for(int m =0 ; m < used; m++){
			MPI_Isend(b, localSize, MPI_DOUBLE, m, 0, MPI_COMM_WORLD, &r[m]);
		}

		for(int m =0 ; m < used; m++){
			int scol = step * m;
			int fcol = step * (m + 1);
			if(fcol > mSize)
				fcol = mSize;
			int colCnt = fcol - scol;
			int antSize = colCnt * mSize;
			// printf("AntSize %d\n", antSize);
			MPI_Recv(buf, antSize, MPI_DOUBLE, m, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			#pragma omp parallel for
			for(int j = 0; j < colCnt; j++){
				for(int i = 0; i < lineCnt; i++){
					for(int k =0; k < mSize; k++){
						int apos = mSize * i + k;
						int bpos = mSize * j + k;
						int cpos = i * mSize + m * step + j;
						c[cpos] += a[apos] * buf[bpos];
					}
				}
			}
		}

		for(int i = 0; i < lineCnt; i++){
			for(int j = 0; j < mSize; j++){
				int pos = i * mSize + j;
				printf("%lf ", c[pos]);
			}
			printf("\n");
		}


		double tack = MPI_Wtime();
		double time = tack - tick;

		if(rank == 0)
			printf("Time %lf\n", time);
		free(a);
		free(b);
		free(c);
		free(buf);
		free(r);
	}

	
	MPI_Finalize();
	return 0;
}