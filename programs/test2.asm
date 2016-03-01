.globl _start
_start:
	addi $t1, $0, 10
	j _skip
	add $t1, $0, $0		# Set t1 to 0
_skip:
	add $v0, $0, $t1	# Set result
_loop:
	j _loop			# Endless loop
