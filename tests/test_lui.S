.globl _start

.text
_start:
	addi $t1, $0, 0x0F	# t1 = 0x0000000F
	lui $t0, 0x01		# t0 = 0x00010000
	addi $t0, $t0, -1	# t0 = 0x0000FFFF
	and $v1, $t1, $t0	# 0x0000FFFF
				# 0x0000000F
			    #   & ----------
				# 0x0000000F

        addi $v0, $0, 10
	syscall
