#!/bin/bash

path=$1
n=$2

gcc -Wall test.c -o test 2> warning.txt
success=$?

grep "warning" warning.txt | sed "s/warning: //g" >> result.txt

if [ $success -eq 0 ]
then
	while [ n -gt 0 ]
	do
		./test
		let "n--"
	done
fi

pwd >> result.txt

