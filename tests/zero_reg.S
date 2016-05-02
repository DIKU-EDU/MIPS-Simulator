.globl _start
.text
_start:
	.set noreorder
	addi $0, $0, 10
	add $v1, $0, $0


	add $v0, $0, 10
	syscall
