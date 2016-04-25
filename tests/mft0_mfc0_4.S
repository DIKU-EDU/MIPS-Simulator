.globl _start

.text
_start:
	.set noreorder
	addi $t0, $0, 10
	mtc0 $t0, $0
#	NOP
	mfc0 $v1, $0


	addi $v0, $0, 10
	syscall
