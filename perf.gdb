# Hardcoded addresses.
# tick() = 0x0000000000403ba1
set pagination off

set variable $i=0
set variable $count=0
while ($i < 1000)
while ($pc != 0x0000000000403ba1)
stepi
set variable $count=1+$count
end
stepi
set variable $i=1+$i
end

p $count
p $i
