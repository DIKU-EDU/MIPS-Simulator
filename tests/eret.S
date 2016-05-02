.globl _start
.text
_start:
	addi $v1, $0, 1
	syscall
	nop
	addi $v1, $t0, 1

	# exit
	addi $v0, $0, 10
	syscall
	nop


.section .exception_handler:
	eret

