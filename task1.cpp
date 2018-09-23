#include <omp.h>
#include <cstdio>
#include <iostream>

using namespace std;

#define VECTOR_SIZE 10

int main(){

	int v[VECTOR_SIZE];

	const int threadNum = 7;
	omp_set_num_threads(threadNum);
	int secSize = VECTOR_SIZE / threadNum + ( VECTOR_SIZE % threadNum > 0);
	
	#pragma omp parallel for
	for( int i = 0; i < VECTOR_SIZE; ++i)
	{
		v[i] = i;
	}

	for( int i = 0; i < VECTOR_SIZE; ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;

	int gm = v[0];
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

	printf("Array max is: %d\n", gm );
	return 0;
}