.globl _start

.data
var1: .word 1234 # Garbage

.text
_start:
	.set noreorder
	sw $0, var1($zero)
	lw $v0, var1($zero)
	syscall
