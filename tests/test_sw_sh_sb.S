.globl _start

.data
var1: .byte  0
var2: .hword 0
var3: .word  0

.text
_start:
	.set noreorder

	addi $t0, $0, 10	# t0 = 10
	addi $t1, $0, 12	# t1 = 12
	addi $t2, $0, 24	# t2 = 24

	sb $t0, var1		# Store t0 in var1
	sh $t1, var2		# Store t1 in var2
	sw $t2, var3		# Store t2 in var3

	# Load values and return sum variables:
	lbu $t0, var1
	lhu $t1, var2
	lw  $t2, var3

	add $v0, $t1, $t0
	add $v0, $v0, $t2

	syscall
