.globl _start

.text
_start:
	.set noreorder
	addi $t0, $0, 10
	mtc0 $t0, $0
#	NOP
	mfc0 $v0, $0
	syscall
