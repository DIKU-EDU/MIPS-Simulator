.text
.globl _start
_start:
	.set noreorder

	la $t0, _start		# t0 = _start
	lw $t1, 0($t0)		# Load first instruction
	lw $t0, cont($0)	# t1 = cont

	# t1 should differ from t0
	bne $t0, $t1, cont	# Jump to cont
	syscall			# error

cont:
	lui $v0, 0xAA		# v0 = 0x00AA0000
	srl $v0, $v0, 16	# v0 = 0xAA
	syscall
