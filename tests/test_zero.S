	.globl _start
	.text
_start:
	.set noreorder

	addi $t0, $0, 5
	addi $0, $0, 10
	# forwarding unit will not bypass $zero, in which case this test will
	# pass
	nop
	nop
	nop
	nop

	add $v0, $t0, $0
	syscall
