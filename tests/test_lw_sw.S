.globl _start

.data
var1:	.byte		16
var2:	.hword 		32
var3:	.word 		48

.text
_start:
	.set noreorder

	add $v0, $0, $0
	lbu $t0, var1
	lhu $t1, var2
	lw $t2, var3

	add $t0, $t0, $t1	# t0 = var1 + var2
	beq $t0, $t2, equal
	nop
	syscall

equal:
	addi $v0, $0, 1		# v0 = 1
	syscall
