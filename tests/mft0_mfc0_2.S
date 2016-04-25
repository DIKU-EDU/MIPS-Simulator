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
	add $v0, $v0, $t0	# v0 = 3


	syscall
