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
		temp="tempfile"
		touch $temp
		echo $count > $temp
		$temp |  ./test $count >> result.txt
		let "count++"
	done
	rm $temp
fi

pwd >> result.txt

