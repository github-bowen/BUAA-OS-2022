#!bin/bash

file=$1
oldString=$2
newString=$3

sed -i "s/${2}/${3}/g" $1


