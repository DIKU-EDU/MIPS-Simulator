.globl _start
_start:
	.set noreorder
	addi $v0, $0, 10

	jal funct
	addi $v1, $v1, 1		# Delay slot: Increments v1
	addi $v1, $v1, 1		# Increment v1
	syscall
	nop
	nop
	nop
	nop



funct:
	addi $v1, $v1, 1 	# Increment v1
	jr $ra			# Return
	nop
	nop
