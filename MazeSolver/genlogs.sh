#!/bin/bash
#
# Script name: genlogs.sh
#
# Description Runs all the possible permutations of amstartup to generate a logfile for each.
#
# Input: none
#
# Output: no direct ouput.  only output from running the maze program
#
# Command line options: none
# 
# Special considerations and assumptions: only works in the Amazing Maze main directory.
#
#

d=0
n=2

$(make clean)
$(make)

while [[ $d -lt 10 ]]; do
	#statements
	n=2
	while [[ $n -lt 11 ]]; do
		#statements

		echo "running $d diff and $n avatars"

		$(./amstartup -d $d -n $n -h flume.cs.dartmouth.edu > /dev/null)

		n=$((n+1));
	done
	d=$((d+1));
done