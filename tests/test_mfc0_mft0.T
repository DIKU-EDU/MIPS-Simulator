.text
.globl _start
_start:
	.set noreorder

	addi $t0, $0, 0x11

	mtc0 $t0, $12		# Move t0 to CP0.Random register.
	mfc0 $v1, $12		# Move CP0.Random register to v1


	syscall			# Return v1
