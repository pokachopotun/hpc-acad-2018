#/bin/bash
for i in 1 2 4 8 ; 
do
	for j in 1 10 100 ;
	do
		mpirun -n $i ./task5 $j > task5_$j.$i.out
	done
done
cat *.out
