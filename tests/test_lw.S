.globl _start

.data

.text
_start:
	.set noreorder
	lw $v0, 0($zero)
	syscall

	# just in case
	nop
	nop
	nop
	nop
	nop

# Actually loads something like 0x3c020040 (first instruction LUI)
