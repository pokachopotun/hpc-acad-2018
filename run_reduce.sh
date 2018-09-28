#/bin/bash
for i in 1 2 4 8 16 ; 
do
	for j in 1 10 100 1000 ;
	do
		mpirun -n $i ./reduce $j > reduce_$j.$i.out
	done
done
cat *.out
