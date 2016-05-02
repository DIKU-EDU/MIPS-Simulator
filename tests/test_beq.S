.globl _start

.text
_start:
	.set noreorder

	addi $t0, $0, 8
	add $t6, $t0, $0
	nop
	nop
	beq $t0, $t6, end
	nop


	addi $v0, $0, 10
	syscall		# This point should not be reached
	nop

end:
	addi $v1, $0, 1

	addi $v0, $0, 10
	syscall
	nop
