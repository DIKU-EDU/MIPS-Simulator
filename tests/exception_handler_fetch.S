.globl _start

.set SYSCALL_EXIT, 0x0A

.section .exception_handler, "ax"
	addi $v1, $v1, 1

	# TODO: This probably needs to be changed in the future
	addi $v0, $0, 10
	syscall

.text
_start:
	addi $v1, $0, 1
	syscall

