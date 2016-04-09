.text
.globl _start
_start:
	.set noreorder
	add $v0, $0, $0		# Reset v0
	jal label1		# Jump to label1
	nop
	nop
	nop
	nop

	la $ra, cont		# Manually set return address
	la $t0, label1		# Set the jump destination
	jr $t0			# Do the jump
	nop
	nop

cont:
	syscall
	nop
	nop
	nop

label1:
	addi $v0, $v0, 1	# Increment v0
	jr $ra
	nop
	nop
	nop
	nop

error:
	syscall
	nop
	nop
