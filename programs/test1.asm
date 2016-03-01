.globl _start
_start:
	addi $t0, $0, 10	# $t0 = 10
	add $t1, $t0, $t0	# $t1 = 20
	add $t2, $t1, $t0	# $t2 = 30
	add $t1, $t2, $t1	# $t1 = 50
	nop
	nop
	nop
