.globl _start
.text
_start:
	.set noreorder
	# Test some forwarding
	# Case MFC0 -> ADDI -> MTC0
	mfc0 $t0, $0
	addi $t0, $t0, 3	# t0 = 3
	mtc0 $t0, $0		# CP0.0 = 3
	nop
	nop
	nop
	nop
	mfc0 $t0, $0 		# t0 should be 3
	add $v1, $v1, $t0	# v1 = 3

	addi $v0, $0, 10
	syscall
