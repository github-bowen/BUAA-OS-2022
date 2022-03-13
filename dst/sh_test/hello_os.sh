#!/bin/bash
srcFile=$1
newFile=$2

touch $2

for i in 8 32 128 512 1024
do
	sed -n "${i}p"  $1 >> $2
done

# the tutorial didn't teach this!!!
