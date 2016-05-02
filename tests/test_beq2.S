.globl _start

.text
_start:
	.set noreorder

	beq $0, $0, end
	nop
	syscall		# This point should not be reached

end:
	addi $v1, $0, 1


	addi $v0, $0, 10
	syscall
