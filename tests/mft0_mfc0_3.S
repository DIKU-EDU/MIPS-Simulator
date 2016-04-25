.globl _start
.text
_start:
	.set noreorder
	# Test some forwarding
	# Case MTC0 -> MFC0
	addi $t0, $0, 2
	mtc0 $t0, $0
	mfc0 $t1, $0		# t1 should be 2
	add $v1, $v1, $t1	# v1 = 2


	addi $v0, $0, 10
	syscall
