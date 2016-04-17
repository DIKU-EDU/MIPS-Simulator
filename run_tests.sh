#!/bin/bash

# Counters
c_failed=0
c=0

# Make the tests directory
make -C tests all

for f in tests/*.elf
do
	c=$((c+1))

	echo -n Running $f
	# Run the program
	 bin/mips-sim -p $f > /dev/null
	# bin/mips-sim -p $f


	# Get return value
	result=$?

	# Read the expected return value
	expected=$(cat "${f%.*}.test")


	if [ "$result" == "$expected" ]; then
		echo " ... PASSED"
	else
		echo " ...FAILED . Expected value: $expected, returned value: $result"
		c_failed=$((c_failed+1))
	fi
done


echo ""
echo "$(($c-$c_failed)) out of $((c)) tests passed."

# Return number of failed tests
exit $c_failed
