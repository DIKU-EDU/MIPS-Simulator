.globl _start
.text
_start:
	.set noreorder
	addi $t0, $0, 1		# t0 = 1

	mtc0 $t0, $0
	mfc0 $t1, $0
	add $v1, $v1, $t1


	addi $v0, $0, 10
	syscall
