# Main makefile
#
# Copyright (C) 2007 Beihang University
# Written by Zhu Like ( zlike@cse.buaa.edu.cn )
#

# ========= Configuration =========

INCLUDES	  := ../include/asm/regdef.h \
				 ../include/asm/cp0regdef.h \
				 ../include/asm/asm.h

# ======== End of configuration ===

%.o: %.S
	$(CC)  $(INCLUDES) -c $< 

.PHONY: clean

all: my_driver.o my_cal.o

clean:
	rm -rf *~ *.o

include ../include.mk


