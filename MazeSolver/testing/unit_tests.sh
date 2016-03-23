#!/bin/bash
# Unit testing for Amazing Maze
# Author: Ella Dzenitis
# CS50, 16W
# 3/8/16

result_file="unit_testing_results.txt"

echo "Unit Testing" > $result_file
echo "" >> $result_file

echo "Argument testing." >> $result_file
cd ..
make
echo "Too many arguments: " >> ./testing/$result_file
amstartup -d 5 4 -n 6 -h flume.cs.dartmouth.edu  >> ./testing/$result_file
echo "" >> ./testing/$result_file
echo "Too few arguments: " >> ./testing/$result_file
amstartup -d 3 -n 2 >> ./testing/$result_file
echo "" >> ./testing/$result_file
echo "Exceeds max depth: " >> ./testing/$result_file
amstartup -d 10 -n 2 -h flume.cs.dartmouth.edu >> ./testing/$result_file
echo "" >> ./testing/$result_file
echo "Exceeds number of allowed avatars: " >> ./testing/$result_file
amstartup -d 5 -n 20 -h flume.cs.dartmouth.edu >> ./testing/$result_file

cd testing

echo "Testing move decision-making." >> $result_file
echo "Created maze where next move (according to left hand rule), should be in the M_WEST direction." >> $result_file
movetest >> $result_file
echo "" >> $result_file

echo "Testing power function." >> $result_file
echo "2^5. Expected: 32" >> $result_file
echo "5^5. Expected: 3125" >> $result_file
echo "3^0. Expected: 1" >> $result_file
ptest >> $result_file
echo "" >> $result_file

echo "Testing maze functionality." >> $result_file
echo "Running: mazetest 5 5 --> creates a 5 by 5 maze" >> $result_file
echo "See maze_testing.txt for results." >> $result_file
mazetest 5 5 


