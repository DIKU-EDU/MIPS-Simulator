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
	addi $v0, $0, 1	# Set result
	syscall

