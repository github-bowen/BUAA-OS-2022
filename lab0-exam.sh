#!/bin/bash

make all
touch hello_os
mkdir hello_os_dir
cp os_hello hello_os_dir/
mv os_hello hello_os_dir/hello_os

grep -n -i "os_hello" hello_os.c >> hello_os.txt

