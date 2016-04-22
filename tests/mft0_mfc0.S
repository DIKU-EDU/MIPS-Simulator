.globl _start
.text
_start:
	.set noreorder

	and $v0, $0, $0
	addi $t0, $0, 1		# t0 = 1

	mtc0 $t0, $0
	mtc0 $t0, $1
	mtc0 $t0, $2
	mtc0 $t0, $3
	mtc0 $t0, $4
	mtc0 $t0, $5
	mtc0 $t0, $6
	mtc0 $t0, $7
	mtc0 $t0, $8
	mtc0 $t0, $9
	mtc0 $t0, $10
	mtc0 $t0, $11
	mtc0 $t0, $12
	mtc0 $t0, $13
	mtc0 $t0, $14
	mtc0 $t0, $15


	mfc0 $t1, $0
	add $v0, $v0, $t1
	mfc0 $t1, $1
	add $v0, $v0, $t1
	mfc0 $t1, $2
	add $v0, $v0, $t1
	mfc0 $t1, $3
	add $v0, $v0, $t1
	mfc0 $t1, $4
	add $v0, $v0, $t1
	mfc0 $t1, $5
	add $v0, $v0, $t1
	mfc0 $t1, $6
	add $v0, $v0, $t1
	mfc0 $t1, $7
	add $v0, $v0, $t1
	mfc0 $t1, $8
	add $v0, $v0, $t1
	mfc0 $t1, $9
	add $v0, $v0, $t1
	mfc0 $t1, $10
	add $v0, $v0, $t1
	mfc0 $t1, $11
	add $v0, $v0, $t1
	mfc0 $t1, $12
	add $v0, $v0, $t1
	mfc0 $t1, $13
	add $v0, $v0, $t1
	mfc0 $t1, $14
	add $v0, $v0, $t1
	mfc0 $t1, $15
	add $v0, $v0, $t1

	syscall
