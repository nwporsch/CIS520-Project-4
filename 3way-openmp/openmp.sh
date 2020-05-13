#!/bin/bash
threads='1 2 4 8 16 32'
ncores='1 2 4 8 16 32'
problem='1000 5000 100000 500000 1000000'
for i in $ncores
do
	for j in $problem
	do
		for k in $threads
		do
			sbatch --constraint=dwarves --mail-type=END --time=1:00:00 --mem=32G --cpus-per-task=$i --output=$j.$k.$i.%J.out start.sh $j $k
		done
	done
done
