.globl _start
.text
_start:
	.set noreorder
	addiu $t0, $0, 10	# $t0 = 10
	add $t1, $t0, $t0	# $t1 = 20
	add $t2, $t1, $t0	# $t2 = 30
	add $t1, $t2, $t1	# $t1 = 50
	addiu $v1, $t1, 20	# $v1 = 50 + 20


	addi $v0, $0, 10
	syscall
