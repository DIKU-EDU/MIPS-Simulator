.globl _start

.section .exceptionhandler
	addi $v0, $v0, 1	# increment
	syscall			# TODO Exit


.text
_start:
	syscall

