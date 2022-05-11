#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>

/*** exercise 3.12 ***/
.section .text.exc_vec3
NESTED(except_vec3, 0, sp)  // exception distributing code
    .set noat
    .set noreorder
1:
    mfc0 k1, CP0_CAUSE    // k1 = CP0_CAUSE
    la k0, exception_handlers  // K0 = base_address(exception_handlers)
    andi k1, 0x7c  // k1 = CP0_CAUSE[6..2] , i.e. ExcCode
    addu k0, k1  // get the address that stores the certain exception_handler function's address
    lw k0, (k0)  
    nop
    jr k0  // jump to this exception_handler 
    nop
END(except_vec3)
	.set at


.data
            .globl mCONTEXT
mCONTEXT:
            .word 0

            .globl delay
delay:
            .word 0

            .globl tlbra
tlbra:
            .word 0

            .section .data.stk
KERNEL_STACK:
            .space 0x8000


            .text
LEAF(_start)

    .set    mips2
    .set    reorder

    /* Disable interrupts */
    mtc0    zero, CP0_STATUS

    /* Disable watch exception. */
    mtc0    zero, CP0_WATCHLO
    mtc0    zero, CP0_WATCHHI

    /* disable kernel mode cache */
    mfc0    t0, CP0_CONFIG
    and    t0, ~0x7
    ori    t0, 0x2
    mtc0    t0, CP0_CONFIG

    /* set up stack */
    li    sp, 0x80400000

    li    t0,0x80400000
    sw    t0,mCONTEXT

    /* jump to main */
    jal    main

loop:
    j    loop
    nop
END(_start)
