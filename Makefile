all: task1, task2

task1:
	g++ task1.cpp -o task1 -fopenmp -lpthread

task2:
	g++ task2.cpp -o task2 -fopenmp -lpthread

tmp:
	g++ tmp.cpp -o tmp -fopenmp -lpthread

task5:
	mpic++ task5.cpp -o task5 -fopenmp -lpthread


last:
	mpic++ last.cpp -o last -fopenmp -lpthread
