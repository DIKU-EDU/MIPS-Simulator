.text
.align 2

.extern init
.globl _start

_start:
	li  $sp, 0x8000fffc

	jal init

	# This point should not be reached
	# If lucky, emergency simulator exit is available with syscall, v0=10
	ori $v0, $0, 10
	syscall

	# Just in case pipeline is broken
	nop
	nop
	nop
	nop
	nop
