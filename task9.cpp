#include <iostream>
#include <mpi.h>
#include <cstdio>
#include <cstdlib>


using namespace std;


int main(int argc, char * argv[]){
	MPI_Init(&argc, &argv);

	// const int iterCount = 3;

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
	for(int i = 0; i < size; i++){
		task[i] = i;
	}

	int color = int(rank >= gSize1);

	MPI_Comm newcomm, comm_cart;
	MPI_Comm_split(MPI_COMM_WORLD, color, 0, &newcomm);

	MPI_Group comm_group, group1, group2;
	int rg1, rg2, trg;

	MPI_Comm_group(MPI_COMM_WORLD, &comm_group);
	
	
	// MPI_Group_incl(comm_group, gSize1, ranks, &group1);
	// MPI_Group_excl(comm_group, gSize1, ranks, &group2);


	// MPI_Group_rank(group2, &rg2);

	// printf("rg1 %d rg2 %d\n",rg1, rg2 );

	int nDims = 1;
	int dims = gSize1;
	int periods = 1;
	if(color == 0){
		MPI_Cart_create(newcomm, nDims, &dims, &periods, 1, &comm_cart);
		MPI_Comm_group(comm_cart, &group1);
		MPI_Group_rank(group1, &rg1);
	}else{
		rg1 = MPI_UNDEFINED;
	}
	printf("rg1 %d\n", rg1);
	if(rg1 != MPI_UNDEFINED){
		printf("global rank %d\n", rank);
		if(gSize1 == 1){
			printf("group 1 rank %d recv %d\n", rg1, rank);	
		}
		else{

			int src, src1;
			int	dst, dst1;
			MPI_Comm_rank(comm_cart, &src);
			int coords;
			MPI_Cart_coords(comm_cart, src, nDims, &coords);
			coords = ( coords + 1) % size;
			MPI_Cart_rank(comm_cart, &coords, &dst);
			MPI_Cart_shift(comm_cart, 0, 1, &src, &dst);
			printf("rank %d sending %d\n", src1, dst1);
			MPI_Isend(&src, 1, MPI_INTEGER, src, dst, comm_cart, &r[trg]);

			MPI_Comm_rank(comm_cart, &src1);
			int coords1;
			MPI_Cart_coords(comm_cart, src1, nDims, &coords1);
			coords1 = ( coords1 - 1 + gSize1 ) % gSize1;

			MPI_Cart_rank(comm_cart, &coords1, &dst1);
			MPI_Cart_shift(comm_cart, 0, -1, &src1, &dst1);

			printf("rank %d receiving %d\n", src1, dst1);
			MPI_Recv(&rnk, 1, MPI_INTEGER, dst1, 0, comm_cart, MPI_STATUS_IGNORE);
		}
	}else if(gSize2 >= 2){
		// MPI_Group_translate_ranks(group2, 1, &rg2, comm_group, &trg);
		// MPI_Group_translate_ranks(group2, gSize2, ranks, comm_group, rec);

		// if(rg2 == 0){
		// 	int curTask = 0;
		// 	for(int j = 0; j < gSize2; j++){
		// 		MPI_Isend(&task[j], 1, MPI_INTEGER, rec[j], 0, MPI_COMM_WORLD, &r[j]);
		// 		curTask++;
		// 	}
		// 	int done = 0;
		// 	while(true){
		// 			printf("group 2 master rank %d, done % d, receiving\n", rg2, done);
		// 			MPI_Recv(&rnk, 1, MPI_INTEGER, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &st[done]);
		// 			task[rnk] = -1;
		// 			done++;
		// 			if(done >= size){
		// 				printf("group 2 master rank %d, leaving\n", rg2);
		// 				break;
		// 			}
		// 			if(curTask < size){
		// 				printf("group 2 master rank %d, sending\n", rg2);
		// 				MPI_Isend(&task[curTask], 1, MPI_INTEGER, st[done - 1].MPI_SOURCE, 0, MPI_COMM_WORLD, &r[size + curTask]);
		// 				curTask++;
		// 			}		
		// 	}
		// 	for(int j = 0; j < gSize2; j++){
		// 			printf("group 2 master rank %d, sending\n", rg2);
		// 			MPI_Isend(&task[j], 1, MPI_INTEGER, rec[j], 0, MPI_COMM_WORLD, &r[size * 2 + j]);
		// 	}
		// 	printf("group 2 master rank %d, shutting down\n", rg2);
		// }else{
		// 	while(true){
		// 		printf("group 2 slave rank %d, receiving\n", rg2);
		// 		MPI_Recv(&rnk, 1, MPI_INTEGER, rec[0], 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// 		if(rnk == -1)
		// 		{
		// 			printf("group 2 slave rank %d, shuttin down\n", rg2);
		// 			break;
		// 		}
		// 		printf("group 2 slave rank %d, task %d done\n", rg2, rnk);
		// 		MPI_Send(&rnk, 1, MPI_INTEGER, rec[0], 0, MPI_COMM_WORLD);
		// 	}
		// }
	}else{
		printf("gSize2  < 2\n");
	}


	
	
	// free(ranks);

	// MPI_Group_free(&comm_group);
	// MPI_Group_free(&group1);
	// MPI_Group_free(&group2);

	// free(r);
	// free(task);

	MPI_Finalize();
	return 0;
}
