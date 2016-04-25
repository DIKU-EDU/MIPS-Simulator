.globl _start
.text
_start:
	.set noreorder
	andi $t0, $0, 10	# $t0 = 0
	addi $t1, $0, 10	# $t1 = 10
	addi $t2, $0, 12	# $t2 = 12
	and $t3, $t1, $t2	# $t3 = 8
	andi $v1, $t3, 0xFF	# $v1 = 8


	addi $v0, $0, 10
	syscall

