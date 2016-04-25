.globl _start
.text
_start:
        .set noreorder
        addiu $t0, $zero, 16
        sll $t1, $t0, 1			# t1 = 32
        srl $t2, $t0, 4			# t2 = 1
        add $v1, $t1, $t2


	add $v0, $0, 10
	syscall
