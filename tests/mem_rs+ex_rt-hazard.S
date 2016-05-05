## A Simultaneous MEM RS and EX RT Hazard ##
#
# An RS hazard with the MEM stage, an RT hazard with the EX stage.
#
# t2 gets t0 + t0
# t3 gets t1 + t1
# v1 gets t2 + t3 (in that order)
#
# t0 and t1 must be non-zero, and different(!) for this to have a clear effect.
addi $t0, $0, 2
addi $t1, $0, 3

add $t2, $t0, $t0 # | IF | ID | EX | MEM |
add $t3, $t1, $t1 #      | IF | ID | EX  |
add $v1, $t2, $t3 #           | IF | ID  | MEM RS hazard + EX RT hazard


# A very permisive exit sequence:
nop
nop
nop
nop

addi $v0, $0, 10
syscall
nop
nop
nop
nop
