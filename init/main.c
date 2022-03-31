/*
 * Copyright (C) 2001 MontaVista Software Inc.
 * Author: Jun Sun, jsun@mvista.com or jsun@junsun.net
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <printf.h>
#include <pmap.h>

typedef struct {
	int size;
	char c;
	int array[3];
} t;

int main()
{
	//printf("main.c:\tmain is start ...\n");
	//mips_init();
	//panic("main is over is error!");
	t t1;
	t1.size = 3;
	t1.c = 'b';
	t1.array[0] = 0;
	t1.array[1] = 1;
	t1.array[2] = 2;

	printf("%T\n", &t1);

	return 0;
}
