#!/bin/bash
FILES="./*"
for file in find $FILES -type f -name "slurm-*"
do
	RESULTS=$(grep ": 0" $file | grep -v "Reading" | tail -1)
	#RESULTS=$(grep -v "Reading" $RESULTS) 
	RESULTS=${RESULTS%:*0}
	RESULTS=${RESULTS#*-}

	THREADS=$(grep "thread" $file)
	THREADS=${THREADS%threads*}
	THREADS=${THREADS#*with}
	#echo $THREADS
	FILE_NAME="$(printf "%s_%s.out" $RESULTS $THREADS)"
	echo $FILE_NAME
	cp $file $FILE_NAME
done
