	.globl _start
	.text
_start:
	.set noreorder
	addi $t1, $0, 10
	j _skip
	nop
	nop
	nop
	nop
	add $t1, $0, $0		# Set t1 to 0
_skip:
	add $v0, $0, $t1	# Set result
	syscall

