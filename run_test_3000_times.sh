#!/bin/bash
# This is because the test checking to see if the maze has a solution
# will never end if there is no solution. Run the test to generate
# 3000 test cases. Typically if an error occures it happens well before the
# three-thousandth test. 
COUNTER=0
while [ $COUNTER -lt 3000 ]; do
	make runtests
	let COUNTER=COUNTER+1
done

make clean
