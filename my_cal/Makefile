# Main makefile
# 
# Copyright (C) 2007 Beihang University
# Written by Zhu Like ( zlike@cse.buaa.edu.cn )
#

# ========= Configuration =========

INCLUDES      := -I../include/

# ======== End of configuration ===

.PHONY: clean

all: my_cal.c my_driver.S
	$(CC) -c my_cal.c -I../include/ -O -G 0 -mno-abicalls -fno-builtin -Wa,-xgot -Wall -fPIC -march=r3000
	$(CC) -c my_driver.S -I../include/ -O -G 0 -mno-abicalls -fno-builtin -Wa,-xgot -Wall -fPIC -march=r3000

clean:
	rm -rf *.o

include ../include.mk                         

