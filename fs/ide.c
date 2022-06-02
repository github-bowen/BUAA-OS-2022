/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

#define IDE_BEGIN_ADDR     0x13000000
#define IDE_OFFSET_ADDR   (IDE_BEGIN_ADDR + 0x0000)
#define IDE_OFFSETHI_ADDR (IDE_BEGIN_ADDR + 0x0008)
#define IDE_ID_ADDR       (IDE_BEGIN_ADDR + 0x0010)
#define IDE_OP_ADDR       (IDE_BEGIN_ADDR + 0x0020)
#define IDE_STATUS_ADDR   (IDE_BEGIN_ADDR + 0x0030)
#define IDE_BUFFER_ADDR   (IDE_BEGIN_ADDR + 0x4000)
#define IDE_BUFFER_SIZE    0x0200
// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurrs during the read of the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
/*** exercise 5.2 ***/
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;

	int cur_offset, op = 0, status;

	while (offset_begin + offset < offset_end) {
		// Your code here
		// error occurred, then panic.
		cur_offset = offset_begin + offset;
		if (syscall_write_dev((u_int) &diskno, IDE_ID_ADDR, 4) < 0) 
			user_panic("fs/ide.c/ide_read(): panic in writing to IDE_ID_ADDR");
		if (syscall_write_dev((u_int) &cur_offset, IDE_OFFSET_ADDR, 4) < 0)
			user_panic("fs/ide.c/ide_read(): panic in writing to IDE_OFFSET_ADDR");
		if (syscall_write_dev((u_int) &op, IDE_OP_ADDR, 4) < 0)
			user_panic("fs/ide.c/ide_read(): panic in writing to IDE_OP_ADDR");
		if (syscall_read_dev((u_int) &status, IDE_STATUS_ADDR, 4) < 0)
			user_panic("fs/ide.c/ide_read(): panic in reading from IDE_STATUS_ADDR");
		if (status == 0)
			user_panic("fs/ide.c/ide_read(): panic for status = 0");
		if (syscall_read_dev((u_int) (dst + offset), IDE_BUFFER_ADDR, 0x200) < 0)
			user_panic("fs/ide.c/ide_read(): panic in reading from IDE_BUFFER_ADDR");
		offset += 0x0200;
	}
}

int time_read() {
	int base = 0x15000000 + 0xa0000000;
	int trigger = base;
	int read = base + 0x0010;
	int time;
	int first = 1;
	do {
		if (first) {
			first = 0;
		} else {
			syscall_write_dev((u_int) &time, trigger, 4);
		}
		syscall_read_dev((u_int) &time, read, 4);
	} while (time == 0);
	return time;
}

// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurrs during the read of the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
/*** exercise 5.2 ***/
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x0200;
	int offset = 0;

	int cur_offset, op = 1, status;

	// DO NOT DELETE WRITEF !!!
	writef("diskno: %d\n", diskno);

	while (offset_begin + offset < offset_end) {
		// copy data from source array to disk buffer.
		// if error occur, then panic.
		cur_offset = offset_begin + offset;
		if (syscall_write_dev((u_int) (src + offset), IDE_BUFFER_ADDR, 0x0200) < 0)
			user_panic("fs/ide.c/ide_write(): panic in writing to IDE_BUFFER_ADDR");
		if (syscall_write_dev((u_int) &diskno, IDE_ID_ADDR, 4) < 0)
			user_panic("fs/ide.c/ide_write(): panic in writing to IDE_ID_ADDR");
		if (syscall_write_dev((u_int) &cur_offset, IDE_OFFSET_ADDR, 4) < 0)
			user_panic("fs/ide.c/ide_write(): panic in writing to IDE_OFFSET_ADDR");
		if (syscall_write_dev((u_int) &op, IDE_OP_ADDR, 4) < 0) 
			user_panic("fs/ide.c/ide_write(): panic in writing to IDE_OP_ADDR");
		if (syscall_read_dev((u_int) &status, IDE_STATUS_ADDR, 4) < 0)
			user_panic("fs/ide.c/ide_write(): panic in reading from IDE_STATUS_ADDR");
		if (status == 0)
			user_panic("fs/ide.c/ide_write(): panic for status = 0");
		offset += 0x0200;
	}
}
