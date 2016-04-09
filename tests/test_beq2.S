.globl _start

.text
_start:
	.set noreorder

	beq $0, $0, end
	nop
	syscall		# This point should not be reached

end:
	addi $v0, $0, 1
	syscall
