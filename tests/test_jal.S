.globl _start
_start:
	.set noreorder

	jal funct
	addi $v0, $v0, 1		# Delay slot: Increments v0
	addi $v0, $v0, 1		# Increment v0
	syscall
	nop
	nop
	nop
	nop



funct:
	addi $v0, $v0, 1 	# Increment v0
	jr $ra			# Return
	nop
	nop
