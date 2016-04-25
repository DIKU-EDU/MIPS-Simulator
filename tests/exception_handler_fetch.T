.globl _start

.set SYSCALL_EXIT, 0x0A

.section .exceptionhandler
	addi $t0, $0, SYSCALL_EXIT
	beq $v1, $t0, _exit

	# huh ?
	# Get stuck
halt:
	j halt


.text
_start:
	syscall


_exit:
	addi $v1, $v1, 1	# increment

	addi $v0, $0, 10
	syscall			# TODO Exit



