#include <omp.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>


#define MAX_MATRIX_SIZE 300

using namespace std;

double a[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
double b[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
double c[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void init(int mSize){

	omp_set_num_threads(4);

	#pragma omp parallel for collapse(2)
	for(int i =0; i < mSize; i++){
		for(int j = 0; j < mSize; j++){
			a[i][j] = fRand(0.001, 1.0);
			b[i][j] = fRand(0.001, 1.0);
			c[i][j] = 0.0;
		}
	}
}

void experiment_ijk(int mSize){

	for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("ijk k-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		
		for(int i =0; i < mSize; i++){
			for(int j = 0; j < mSize; j++){
				double s = 0;
				#pragma omp parallel for reduction(+ : s)
				for(int k =0 ; k < mSize; k++){
					 s += a[i][k] * b[k][j];
				}
				c[i][j] = s;
	 		}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
	for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("ijk j-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		
		for(int i =0; i < mSize; i++){
			#pragma omp parallel for
			for(int j = 0; j < mSize; j++){
				for(int k =0 ; k < mSize; k++){
					 c[i][j] += a[i][k] * b[k][j];
				}
	 		}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
	for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("ijk i-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		
		#pragma omp parallel for
		for(int i =0; i < mSize; i++){
			for(int j = 0; j < mSize; j++){
				for(int k =0 ; k < mSize; k++){
					 c[i][j] += a[i][k] * b[k][j];
				}
	 		}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
		for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("ijk ij-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		
		#pragma omp parallel for collapse(2)
		for(int i =0; i < mSize; i++){
			for(int j = 0; j < mSize; j++){
				for(int k =0 ; k < mSize; k++){
					 c[i][j] += a[i][k] * b[k][j];
				}
	 		}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
}

void experiment_jik(int mSize){

	for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("jik k-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		
		for(int j = 0; j < mSize; j++){		
			for(int i =0; i < mSize; i++){
				double s = 0;
				#pragma omp parallel for reduction(+ : s)
				for(int k =0 ; k < mSize; k++){
					 s += a[i][k] * b[k][j];
				}
				c[i][j] = s;
	 		}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
	for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("jik i-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		
		for(int j = 0; j < mSize; j++){
			#pragma omp parallel for
			for(int i =0; i < mSize; i++){
				for(int k =0 ; k < mSize; k++){
					 c[i][j] += a[i][k] * b[k][j];
				}
	 		}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
	for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("jik j-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		
		#pragma omp parallel for
		for(int j = 0; j < mSize; j++){
			for(int i =0; i < mSize; i++){
				for(int k =0 ; k < mSize; k++){
					 c[i][j] += a[i][k] * b[k][j];
				}
	 		}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
		for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("jik ij-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		
		#pragma omp parallel for collapse(2)
		for(int j = 0; j < mSize; j++){
			for(int i =0; i < mSize; i++){
				for(int k =0 ; k < mSize; k++){
					 c[i][j] += a[i][k] * b[k][j];
				}
	 		}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
}


void experiment_ikj(int mSize){

	// for(int threadNum = 1; threadNum <= 4; threadNum++){
	// 	printf("ikj k-parallel; threads %d; ", threadNum);
	// 	init(mSize);
	// 	omp_set_num_threads(threadNum);

	// 	double tick = omp_get_wtime();
	// 	for(int i =0; i < mSize; i++){
	// 		for(int k =0 ; k < mSize; k++){
	// 			for(int j = 0; j < mSize; j++){		
	// 				 c[i][j] += a[i][k] * b[k][j];
	// 			}
	//  		}
	// 	}

	// 	double tack = omp_get_wtime();
	// 	double time = tack - tick;
	// 	printf("time is: %f\n", time);
	// }
	// printf("\n");
	for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("ikj i-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		#pragma omp parallel for
		for(int i =0; i < mSize; i++){
			for(int k =0 ; k < mSize; k++){
				for(int j = 0; j < mSize; j++){		
					 c[i][j] += a[i][k] * b[k][j];
				}
			}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
	for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("ikj j-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		
		
		for(int i =0; i < mSize; i++){
			for(int k =0 ; k < mSize; k++){
				#pragma omp parallel for
				for(int j = 0; j < mSize; j++){		
					c[i][j] += a[i][k] * b[k][j];
				}
	 		}

		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");

	for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("ikj ik-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();

		double s  = 0;
		#pragma omp parallel for collapse(2)
		for(int i =0; i < mSize; i++){
			for(int k =0 ; k < mSize; k++){
				for(int j = 0; j < mSize; j++){
			 		c[i][j] += a[i][k] * b[k][j];
				}
	 		}
		}
		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
}

void experiment_kij(int mSize){

	// for(int threadNum = 1; threadNum <= 4; threadNum++){
	// 	printf("kij k-parallel; threads %d; ", threadNum);
	// 	init(mSize);
	// 	omp_set_num_threads(threadNum);

	// 	double tick = omp_get_wtime();
		
	// 	for(int k =0 ; k < mSize; k++){
	// 		for(int i =0; i < mSize; i++){
	// 			for(int j = 0; j < mSize; j++){
	// 				 c[i][j] += a[i][k] * b[k][j];
	// 			}
	//  		}
	// 	}

	// 	double tack = omp_get_wtime();
	// 	double time = tack - tick;
	// 	printf("time is: %f\n", time);
	// }
	printf("\n");
	for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("kij j-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		
		for(int k =0 ; k < mSize; k++){
			for(int i =0; i < mSize; i++){
				#pragma omp parallel for
				for(int j = 0; j < mSize; j++){
					 c[i][j] += a[i][k] * b[k][j];
				}
	 		}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
	for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("kij i-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		

		for(int k =0 ; k < mSize; k++){
			#pragma omp parallel for
			for(int i =0; i < mSize; i++){
				for(int j = 0; j < mSize; j++){
					 c[i][j] += a[i][k] * b[k][j];
				}
	 		}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
		for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("kij ij-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		
		for(int k =0 ; k < mSize; k++){
			#pragma omp parallel for collapse(2)
			for(int i =0; i < mSize; i++){
				for(int j = 0; j < mSize; j++){
					 c[i][j] += a[i][k] * b[k][j];
				}
	 		}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
}

void experiment_jki(int mSize){
	// for(int threadNum = 1; threadNum <= 4; threadNum++){
	// 	printf("jki k-parallel; threads %d; ", threadNum);
	// 	init(mSize);
	// 	omp_set_num_threads(threadNum);

	// 	double tick = omp_get_wtime();
		
	// 	for(int j = 0; j < mSize; j++){
	// 		for(int k =0 ; k < mSize; k++){
	// 			for(int i =0; i < mSize; i++){
	// 				 c[i][j] += a[i][k] * b[k][j];
	// 			}
	//  		}
	// 	}

	// 	double tack = omp_get_wtime();
	// 	double time = tack - tick;
	// 	printf("time is: %f\n", time);
	// }
	// printf("\n");
	for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("jki j-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		
		#pragma omp parallel for
		for(int j = 0; j < mSize; j++){
			for(int k =0 ; k < mSize; k++){
				for(int i =0; i < mSize; i++){
					 c[i][j] += a[i][k] * b[k][j];
				}
	 		}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
	for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("jki i-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		

		for(int j = 0; j < mSize; j++){
			for(int k =0 ; k < mSize; k++){
				#pragma omp parallel for
				for(int i =0; i < mSize; i++){
					 c[i][j] += a[i][k] * b[k][j];
				}
	 		}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
		for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("jki jk-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		
		#pragma omp parallel for collapse(2)
		for(int j = 0; j < mSize; j++){
			for(int k =0 ; k < mSize; k++){
				for(int i =0; i < mSize; i++){
					 c[i][j] += a[i][k] * b[k][j];
				}
	 		}
		}
		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
}

void experiment_kji( int mSize){

	// for(int threadNum = 1; threadNum <= 4; threadNum++){
	// 	printf("kji k-parallel; threads %d; ", threadNum);
	// 	init(mSize);
	// 	omp_set_num_threads(threadNum);

	// 	double tick = omp_get_wtime();
		
	// 	for(int k =0 ; k < mSize; k++){
	// 		for(int j = 0; j < mSize; j++){	
	// 			for(int i =0; i < mSize; i++){
	// 				 c[i][j] += a[i][k] * b[k][j];
	// 			}
	//  		}
	// 	}

	// 	double tack = omp_get_wtime();
	// 	double time = tack - tick;
	// 	printf("time is: %f\n", time);
	// }
	// printf("\n");
	for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("kji j-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		
		for(int k =0 ; k < mSize; k++){
			#pragma omp parallel for
			for(int j = 0; j < mSize; j++){	
				for(int i =0; i < mSize; i++){
					 c[i][j] += a[i][k] * b[k][j];
				}
	 		}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
	for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("kji i-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		

		for(int k =0 ; k < mSize; k++){
			for(int j = 0; j < mSize; j++){
				#pragma omp parallel for	
				for(int i =0; i < mSize; i++){
					 c[i][j] += a[i][k] * b[k][j];
				}
	 		}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
		for(int threadNum = 1; threadNum <= 4; threadNum++){
		printf("kji ij-parallel; threads %d; ", threadNum);
		init(mSize);
		omp_set_num_threads(threadNum);

		double tick = omp_get_wtime();
		
		for(int k =0 ; k < mSize; k++){
			#pragma omp parallel for collapse(2)
			for(int j = 0; j < mSize; j++){	
				for(int i =0; i < mSize; i++){
					 c[i][j] += a[i][k] * b[k][j];
				}
	 		}
		}

		double tack = omp_get_wtime();
		double time = tack - tick;
		printf("time is: %f\n", time);
	}
	printf("\n");
}


void experiment(int mSize){
	printf("mSize: %d\n", mSize);
	experiment_ijk(mSize);
	experiment_jik(mSize);
	experiment_ikj(mSize);
	experiment_kij(mSize);
	experiment_jki(mSize);
	experiment_kji(mSize);
}


int main(){

	
		for(int mSize = MAX_MATRIX_SIZE; mSize <= MAX_MATRIX_SIZE; mSize *= 10){
			experiment(mSize);
		}
		
	return 0;
}