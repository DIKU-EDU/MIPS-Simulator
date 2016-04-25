.globl _start
.text
_start:
        .set noreorder
        addiu $t0, $zero, 24		# t0 = 24
	or $t1, $zero, $t0		# t1 = t0
        ori $t2, $t1, 1			# t2 = 25

	nor $t3, $0, $0			# t3 = 0xFFFFFFFF
	nor $t4, $0, $0			# t4 = 0xFFFFFFFF
	sll $t3, $t3, 8			# t3 = 0xFFFFFF00
        sll $t4, $t4, 4			# t4 = 0xFFFFFFF0
        nor $t3, $t3, $t4		# t3 = 0x0000000F

	add $v1, $t2, $t3		# v1 = 40



	add $v0, $0, 10
        syscall
