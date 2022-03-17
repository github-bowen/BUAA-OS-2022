#!/bin/bash
path=$1
n=$2
count=1

gcc -Wall test.c -o test 2> warning.txt
success=$?

grep "warning" warning.txt | sed "s/warning: //g" >> result.txt

if [ $success -eq 0 ]
then
	while [ $count -le $n ]
	do
		echo $count |  ./test >> result.txt
		let "count++"
	done
fi

pwd >> result.txt

