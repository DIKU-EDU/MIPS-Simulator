## A Simultaneous MEM RT and EX RS Hazard ##
#
# An RT hazard with the MEM stage, an RS hazard with the EX stage.
#
# t2 gets t0 + t0
# t3 gets t1 + t1
# v1 gets t3 + t2 (in that order)
#
# t0 and t1 must be non-zero, and different(!) for this to have a clear effect.
addi $t0, $0, 2
addi $t1, $0, 3

add $t2, $t0, $t0 # | IF | ID | EX | MEM | (the RT register is in MEM)
add $t3, $t1, $t1 #      | IF | ID | EX  | (the RS register is in EX)
add $v1, $t3, $t2 #           | IF | ID  | MEM RT hazard + EX RS hazard


# A very permisive exit sequence:
nop
nop
nop
nop
addi $v0, $0, 10		# exit
syscall
nop
nop
nop
nop
