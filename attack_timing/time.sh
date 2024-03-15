#!/bin/bash

laptop="LENOVO - LOAD 2"

filename="timings.txt"

echo $laptop >> $filename


make

./attack >> $filename
./attack_ntmp >> $filename
./rh4_test >> $filename
./rh8_test >> $filename
./rh9_test >> $filename
./rh10_test >> $filename
./rh20_test >> $filename
./double_sided_rowhammer >> $filename

make clean


echo "\n" >> $filename
