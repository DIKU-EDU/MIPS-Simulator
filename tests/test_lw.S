.data
var1: .word 0xC
var2: .word 0x6

.text
.globl _start
_start:
	.set noreorder
	lw $t0, var1
	lw $t1, var2
	and $v0, $t0, $t1	# 0xC & 0x6 = 0x4
	syscall
