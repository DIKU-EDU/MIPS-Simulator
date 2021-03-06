.globl _start
.text
_start:
	.set noreorder

	addi $t0, $0, 0xA
	addi $t1, $0, 0xC

	and $t1, $t1, $t0	# $t1 = 0xA & 0xC = 0x8


	srl $t1, $t1, 0x1	# $t1 = 0100 = 0x4
	andi $v1, $t1, 0x5	# $v0 = 0100 & 0110 = 0100 = 4


	# Basic memory operation
	lui $t0, 0x8000
	ori $t0, 0x0000

	lui $t1, 0xAAAA
	ori $t1, $t1, 0xBBBB

	sw $t1, 0($t0)		# Store 0xAAAABBBB at 0x80000000


	# Move 0xAAAABBBB into CoProcessor PRId (Product ID) register
	mtc0 $t1, $18



	# exit
	addi $v0, $0, 10
	syscall

