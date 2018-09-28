#include <iostream>
#include <mpi.h>
#include <cstdio>
#include <cstdlib>


using namespace std;


#define MAX_NAME_LENGTH 100
#define BUF_SIZE 256

int main(int argc, char * argv[]){
	MPI_Init(&argc, &argv);

	// int n = atoi(argv[1]);
	int size, rank, rnk;
	char buf[BUF_SIZE];
	int position;

	MPI_Comm_rank(MPI_COMM_WORLD,  &rank);
	MPI_Comm_size(MPI_COMM_WORLD,  &size);
	rnk = rank;

	char name[MAX_NAME_LENGTH];
	int nameLen;

	MPI_Get_processor_name(name, &nameLen);
	sprintf(&name[nameLen], "_id_%d", rank);

	MPI_Pack(&rank, 1, MPI_INTEGER, buf, BUF_SIZE, &position, MPI_COMM_WORLD);
	MPI_Pack(&nameLen, 1, MPI_INTEGER, buf, BUF_SIZE, &position, MPI_COMM_WORLD);
	MPI_Pack(name, MAX_NAME_LENGTH, MPI_CHAR, buf, BUF_SIZE, &position, MPI_COMM_WORLD);
	
	if(rank != 0)
		MPI_Send(buf, BUF_SIZE, MPI_PACKED, 0, 0, MPI_COMM_WORLD);
	else
		for(int i =0 ; i < size; i++){
			if(i > 0)
				MPI_Recv(buf, BUF_SIZE, MPI_PACKED, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			int upos = 0;
			MPI_Unpack(buf, BUF_SIZE, &upos, &rnk, 1, MPI_INTEGER, MPI_COMM_WORLD);
			MPI_Unpack(buf, BUF_SIZE, &upos, &nameLen, 1, MPI_INTEGER, MPI_COMM_WORLD);
			MPI_Unpack(buf, BUF_SIZE, &upos, &name, MAX_NAME_LENGTH, MPI_CHAR, MPI_COMM_WORLD);
			printf("rank %d %s\n", rnk, name);
		}
	MPI_Finalize();
	return 0;
}
