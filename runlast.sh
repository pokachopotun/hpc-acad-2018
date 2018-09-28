#/bin/bash
for nNodes in 1 2 4 8 ; 
do
	for mSize in 10 ;
	do
		mpirun -n $nNodes ./last $mSize > task5_$mSize.$nNodes.out
	done
done
cat *.out
