.globl _start
.text
_start:
	addi $v1, $0, 1
	syscall
	nop
	addi $v1, $v1, 1

	# exit
	addi $v0, $0, 10
	syscall
	nop


.section .exception_handler, "ax"
	mfc0 $k0, $14     # get EPC in $k0
	addiu $k0, 4      # make sure it points to next instruction
	mtc0 $k0, $14

	addi $v1, $v1, 1
	eret

