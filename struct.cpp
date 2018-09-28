#include <iostream>
#include <mpi.h>
#include <cstdio>
#include <cstdlib>


using namespace std;


#define MAX_NAME_LENGTH 256

int main(int argc, char * argv[]){
	MPI_Init(&argc, &argv);

	// int n = atoi(argv[1]);
	int size, rank, rnk;

	MPI_Comm_rank(MPI_COMM_WORLD,  &rank);
	MPI_Comm_size(MPI_COMM_WORLD,  &size);
	rnk = rank;

	char name[MAX_NAME_LENGTH];
	int nameLen;

	MPI_Get_processor_name(name, &nameLen);

	sprintf(&name[nameLen], "_id_%d", rank);

	int blocklens[3];
	MPI_Datatype types[3];
	MPI_Aint displs[3];
	blocklens[0] = 1;
	blocklens[1] = 1;
	blocklens[2] = MAX_NAME_LENGTH;
	types[0] = MPI_INTEGER;
	types[1] = MPI_INTEGER;
	types[2] = MPI_CHAR;


	MPI_Address(&rnk, &displs[0]);
	MPI_Address(&nameLen, &displs[1]);
	MPI_Address(name, &displs[2]);
	MPI_Datatype newtype;	
	
	MPI_Type_struct(3, blocklens, displs, types, &newtype);
	MPI_Type_commit(&newtype);
	if(rank != 0)
		MPI_Send(MPI_BOTTOM, 1, newtype, 0, 0, MPI_COMM_WORLD);
	if(rank == 0)
	for(int i =0 ; i < size; i++){
		if(i > 0)
			MPI_Recv(MPI_BOTTOM, 1, newtype, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("rank %d %s\n", rnk, name);
	}
	MPI_Finalize();
	return 0;
}
