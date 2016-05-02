.globl _start

.data
var1: .word 1234 # Garbage

.text
_start:
	.set noreorder
	sw $0, var1($zero)
	lw $v1, var1($zero)


	add $v0, $0, 10
	syscall
