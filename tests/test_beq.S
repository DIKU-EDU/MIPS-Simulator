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

	syscall		# This point should not be reached
	nop

end:
	addi $v0, $0, 1
	syscall
	nop
