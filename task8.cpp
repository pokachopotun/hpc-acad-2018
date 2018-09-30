#include <iostream>
#include <mpi.h>
#include <cstdio>
#include <cstdlib>


using namespace std;


int main(int argc, char * argv[]){
	MPI_Init(&argc, &argv);

	const int iterCount = 3;

	// int n = atoi(argv[1]);
	int size, rank, rnk;

	MPI_Comm_rank(MPI_COMM_WORLD,  &rank);
	MPI_Comm_size(MPI_COMM_WORLD,  &size);


	MPI_Request * r = (MPI_Request * ) calloc(size * 3, sizeof(MPI_Request));
	MPI_Status * st = (MPI_Status * ) calloc(size * 3, sizeof(MPI_Status));
	int * rec = (int * ) calloc( size, sizeof(int));
	int * ranks = (int * ) calloc( size , sizeof(int));
	int * task = (int * )calloc(size, sizeof(int));
	int gSize1 = size / 2;

	int gSize2 = size - gSize1;
	// if(rank == 0)
	// 	printf("size1 % d size2 %d \n", gSize1, gSize2);
	for(int i = 0;i < size; i++){
		ranks[i] = i;
		task[i] = i;
	}
	MPI_Group comm_group, group1, group2;
	int rg1, rg2, trg;

	MPI_Comm_group(MPI_COMM_WORLD, &comm_group);
	MPI_Group_incl(comm_group, gSize1, ranks, &group1);
	MPI_Group_excl(comm_group, gSize1, ranks, &group2);

	MPI_Group_rank(group1, &rg1);
	MPI_Group_rank(group2, &rg2);

	// printf("rg1 %d rg2 %d\n",rg1, rg2 );
	

	if(rg1 != MPI_UNDEFINED){
		MPI_Group_translate_ranks(group1, 1, &rg1, comm_group, &trg);
		MPI_Isend(&rg1, 1, MPI_INTEGER, (trg + 1) % gSize1, 0, MPI_COMM_WORLD, &r[trg]);
		MPI_Recv(&rnk, 1, MPI_INTEGER, (gSize1 + trg - 1) % gSize1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("group 1 rank %d recv %d\n", rg1, rnk);	
	}else if(gSize2 >= 2){
		MPI_Group_translate_ranks(group2, 1, &rg2, comm_group, &trg);
		MPI_Group_translate_ranks(group2, gSize2, ranks, comm_group, rec);

		if(rg2 == 0){
			int curTask = 0;
			for(int j = 0; j < gSize2; j++){
				MPI_Isend(&task[j], 1, MPI_INTEGER, rec[j], 0, MPI_COMM_WORLD, &r[j]);
				curTask++;
			}
			int done = 0;
			while(true){
					printf("group 2 master rank %d, done % d, receiving\n", rg2, done);
					MPI_Recv(&rnk, 1, MPI_INTEGER, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &st[done]);
					task[rnk] = -1;
					done++;
					if(done >= size){
						printf("group 2 master rank %d, leaving\n", rg2);
						break;
					}
					if(curTask < size){
						printf("group 2 master rank %d, sending\n", rg2);
						MPI_Isend(&task[curTask], 1, MPI_INTEGER, st[done - 1].MPI_SOURCE, 0, MPI_COMM_WORLD, &r[size + curTask]);
						curTask++;
					}		
			}
			for(int j = 0; j < gSize2; j++){
					printf("group 2 master rank %d, sending\n", rg2);
					MPI_Isend(&task[j], 1, MPI_INTEGER, rec[j], 0, MPI_COMM_WORLD, &r[size * 2 + j]);
			}
			printf("group 2 master rank %d, shutting down\n", rg2);
		}else{
			while(true){
				printf("group 2 slave rank %d, receiving\n", rg2);
				MPI_Recv(&rnk, 1, MPI_INTEGER, rec[0], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				if(rnk == -1)
				{
					printf("group 2 slave rank %d, shuttin down\n", rg2);
					break;
				}
				printf("group 2 slave rank %d, task %d done\n", rg2, rnk);
				MPI_Send(&rnk, 1, MPI_INTEGER, rec[0], 0, MPI_COMM_WORLD);
			}
		}
	}else{
		printf("gSize2  < 2\n");
	}


	
	
	free(ranks);

	MPI_Group_free(&comm_group);
	MPI_Group_free(&group1);
	MPI_Group_free(&group2);

	free(r);
	free(task);

	MPI_Finalize();
	return 0;
}
