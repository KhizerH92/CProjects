#!/bin/bash

# This is the Lab6 QEBATS.sh file to test our Query

#Description: This script is basically going to do a number of runs on the Query
#			  We will do clean builds, wrong number of arguments checks, wrong 
#			  arguments but proper number of arguments check.
#
# Results: Will be written to a 'log File' that will be submitted along with the code:

filename=./QueryEngine/Logfile.txt
buildTime=`date`
targetDir=../crawler/
indexFileLoc=./indexer/index3.dat
mainFile=./QueryEngine/Query

echo > $filename

echo "Running Query TESTS now. Please view $filename for Results"

echo "The test was done at $buildTime" >> "$filename"
echo " " >> "$filename"

echo "Following are the results for various tests being done: " >> "$filename"
echo " " >> "$filename"
echo "Doing a clean build leads to " >> "$filename"
echo " " >> "$filename"
make clean >> "$filename"
make Query >> "$filename"

if [ $? -ne 0 ]; then
	echo "build failed" >> "$filename"
	exit 1
fi
echo " " >> "$filename"
echo "++++++++++++++++++++++++++++++++" >> "$filename"

echo " " >> "$filename"
echo "After clen build, test wrong number of arguments = fewer arguments" >> "$filename"
echo " " >> "$filename"
$mainFile ./data/crawler >> "$filename"
echo " " >> "$filename"
echo "++++++++++++++++++++++++++++++++" >> "$filename"

echo " " >> "$filename"
echo "After clen build, test wrong number of arguments = more arguments" >> "$filename"
echo " " >> "$filename"
$mainFile $indexFileLoc $targetDir $targetDir >> "$filename"
echo " " >> "$filename"
echo "++++++++++++++++++++++++++++++++" >> "$filename"

echo " " >> "$filename"
echo "After clen build, test wrong number of arguments = more arguments" >> "$filename"
echo " " >> "$filename"
$mainFile $indexFileLoc ../data/ >> "$filename"
echo " " >> "$filename"
echo "++++++++++++++++++++++++++++++++" >> "$filename"

echo " " >> "$filename"
echo "Right nummber of arguments but wrong files leads to: " >> "$filename"
echo " " >> "$filename"
$mainFile indexfilewhat.dat $targetDir  >> "$filename"
echo " " >> "$filename"
echo "++++++++++++++++++++++++++++++++" >> "$filename"

make clean &> /dev/null
