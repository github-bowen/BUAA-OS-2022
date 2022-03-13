#!bin/bash
#First you can use grep (-n) to find the number of lines of string.
#Then you can use awk to separate the answer.
file=$1
string=$2
newfile=$3

grep -n $string $file | awk -F: '{print $1}' >> $newfile



