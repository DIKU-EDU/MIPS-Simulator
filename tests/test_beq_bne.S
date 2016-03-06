.text
.globl _start
_start:
	.set noreorder
	addi $t0, $0, 14	# t0 = 14
	add $t1, $0, $t0	# t1 = 14
	addi $t2, $0, 12	# t2 = 12

	beq $t0, $t1, equal	# jump to equal
	nop
	j error			# This point should not be reached

equal:
	addi $v0, $0, 1		# v0 = 1
	bne $t0, $t2, not_equal
	nop
	j error			# This point should not be reached


not_equal:
	addi $v0, $v0, 1	# v0 = 2
	syscall

error:
	addi $v0, $0, 0
	syscall
