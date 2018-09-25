#include <omp.h>
#include <cstdio>
#include <iostream>

using namespace std;

#define VECTOR_SIZE 10000000

int main(){

	int v[VECTOR_SIZE];

	mp_set_num_threads(4);
	#pragma omp parallel for
	for( int i = 0; i < VECTOR_SIZE; ++i)
	{
		v[i] = i;
	}

	for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("threadNum = %d\n", threadNum);
		omp_set_num_threads(threadNum);
		int secSize = VECTOR_SIZE / threadNum + ( VECTOR_SIZE % threadNum > 0);

		int gm = v[0];
		double tick = omp_get_wtime();
		#pragma omp parallel
		{	
			int procId = omp_get_thread_num();
			int s = procId * secSize;
			if( s < VECTOR_SIZE ){
				int f = min( (procId + 1) * secSize, VECTOR_SIZE);
				int lm = v[s];
				for(int i = s; i < f; i++){
					lm = max(lm, v[i]);
				}
				printf("%d %d max is: %d\n", s, f, lm);
				#pragma omp critical
				if(lm > gm){
					gm = lm;
				}
			}
		}
		double tack = omp_get_wtime();
		double time = tick - tack;
		printf("Array max is: %d\n", gm );
		printf("Time is: %d\n", time );
	}
	return 0;
}