all: task1

task1:
	g++ -std=c++11 task1.cpp -o test -fopenmp -lpthread