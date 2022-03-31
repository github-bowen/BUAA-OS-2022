#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>

// my_driver.S

// char _my_getchar()

LEAF(_my_getchar)

start:

	li s0, 0x10000000  // s0 base addr
	addi t0, 48 // t0 = '0'
	lb s1, 0x00(s0)  // s1 = read-in char
	
	beq t0, s1, start
	nop

	sb s1, 0x00(s0)
	move v0, s1
	jr ra
	nop
END(_my_getchar)

LEAF(_my_putchar)

	move s2, a0  // s2 = char to put
	li s0, 0x10000000
	sb s2, 0x00(s0)
	jr ra
	nop
END(_my_putchar)

LEAF(_my_exit)

	li s0, 0x10000000
	sb zero, 0x10(s0)

END(_my_exit)
