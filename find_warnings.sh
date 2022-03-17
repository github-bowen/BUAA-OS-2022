#!/bin/bash

path=$1
n=$2
i=1

gcc -Wall test.c -o test 2> warning.txt
success=$?

grep "warning" warning.txt | sed "s/warning: //g" >> result.txt

if [ $success -eq 0 ]
then
	while [ $i -le $n ]
	do
		./test $i  >> result.txt
		let "i++"
	done
fi

pwd >> result.txt

