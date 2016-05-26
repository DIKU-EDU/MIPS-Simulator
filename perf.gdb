# Hardcoded addresses. Probably use tick() address

set pagination off
b *0x00000000004036fc
run

set variable $i=0
set variable $count=0

while ($i < 1000)
while ($pc != 0x00000000004036fc)
stepi
set variable $count=1+$count
end
stepi
set variable $i=1+$i
end


p $count
p $i
