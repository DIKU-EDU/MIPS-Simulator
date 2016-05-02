	.globl _start
	.text
_start:
	.set noreorder
	j _skip
	nop
	nop
	syscall
	nop
_skip:
	addi $v1, $0, 1	# Set result

	addi $v0, $0, 10
	syscall

