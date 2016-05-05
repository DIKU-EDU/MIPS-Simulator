## MEM ALU Result Data Hazard ##
#
# The value needed in EX stage has only just passed MEM. In this case, it is
# the data computed in EX that needs to be forwarded.
#
# t0 should be non-zero for this to work.
#
# t1 should get t0 + t0
# t2 should get t0 + t0 + t0 + t0

addi $t0, $0, 5

add $t1, $t0, $t0 # | IF | ID | EX | MEM | t1 reached MEM stage
nop               #      | IF | ID | EX  |
add $t2, $t1, $t1 #           | IF | ID  | MEM RS + MEM RT hazard


# A very permisive exit sequence:
nop
nop
nop
nop
addi $v1, $t2, $0
addi $v0, $0, 10		# exit
syscall
nop
nop
nop
nop
