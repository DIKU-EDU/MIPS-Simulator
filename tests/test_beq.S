.globl _start

.text
_start:
	.set noreorder

	addi $t0, $0, 8
	add $t6, $t0, $0

	beq $t0, $t6, end
	nop
	nop
	syscall		# This point should not be reached
	nop
	nop
	nop


end:
	addi $v0, $0, 1
	syscall
	nop
