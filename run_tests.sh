#!/bin/bash

# Counters
c_failed=0
c=0

# Make the tests directory
cd tests
make all
cd ..

for f in tests/*.elf
do
	c=$((c+1))

	# Run the program
	bin/mips-sim -p $f > /dev/null

	# Get return value
	result=$?

	# Read the expected return value
	expected=$(cat "${f%.*}.test")


	if [ "$result" == "$expected" ]; then
		echo PASSED: $f
	else
		echo FAILED $f. Expected value: $expected, returned value: $result
		c_failed=$((c_failed+1))
	fi
done


echo ""
echo "$(($c-$c_failed)) out of $((c)) tests passed."

