.data
var0: .word 32
var1: .word 17

.text
.globl _start



_start:
        .set noreorder

        addi $t0, $0, 18

        la $t1, var1   	# Load address of var1 into t1
	ll $t2, 0($t1)	# Load var1 into t2
	sc $t0, 0($t1)	# Store t0 into var1, t0 should be 1

	beq $t0, 1, equal

	# This point should not be reached. Exit
	addi $v0, $0, 1
	syscall

equal:
	# LL var0  and try to store to var1
	la $t1, var0
	ll $t2, 0($t1)
	la $t0, var1
	sc $t1, 0($t0)

	beq $t1, 0, success

	# This point should not be reached.
	addi $v0, $0, 0
        syscall

success:
	addi $v0, $0, 1
	syscall
