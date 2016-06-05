.globl _start
.text
_start:
	.set noreorder
	addi $t0, $0, 0xA
	addi $t1, $0, 0xC

	and $t1, $t1, $t0	# 	0xA
				#	0xC
				# AND -----
				#       0x8 = 1000


	srl $t1, $t1, 0x1	# $t1 = 0100 = 0x4

	andi $v1, $t1, 0x5

	# exit
	addi $v0, $0, 10
	syscall

