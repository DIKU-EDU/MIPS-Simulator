.data
var1: .word 0
.text
.globl _start
_start:
	.set noreorder
	lui $t1, 0x1234
	ori $t1, 0x5678
	sw $t1, var1
	lw $t0, var1

	andi $v1, $t0, 0xFF	# v1 = 0x78


	addi $v0, $0, 10
	syscall
