.globl _start

.set SYSCALL_EXIT, 0x0A

.section .exceptionhandler
	addi $t0, $0, SYSCALL_EXIT
	beq $v0, $t0, _exit

	# huh ?
	# Get stuck
halt:
	j halt


.text
_start:
	syscall


_exit:
	addi $v0, $v0, 1	# increment
	syscall			# TODO Exit


