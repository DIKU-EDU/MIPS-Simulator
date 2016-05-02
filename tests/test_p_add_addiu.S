.globl _start
.text
_start:
	.set noreorder
	addiu $t0, $0, 10	# $t0 = 10
	nop
	nop
	nop
	nop
	add $t1, $t0, $t0	# $t1 = 20
	nop
	nop
	nop
	nop


	add $t2, $t1, $t0	# $t2 = 30
	nop
	nop
	nop
	nop

	add $t1, $t2, $t1	# $t1 = 50
	nop
	nop
	nop
	nop

	addiu $v1, $t1, 20	# $v1 = 50 + 20
	nop
	nop
	nop
	nop

	add $v0, $0, 10
	syscall
