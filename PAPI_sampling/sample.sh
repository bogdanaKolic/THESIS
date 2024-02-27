#!/bin/sh

NUM_RUNS=20

sudo make

while [ $NUM_RUNS -gt 0 ]
do
    sudo ./attack_sampled >> error_no_mult.csv
    sudo ./attack_benchmark >> error_no_mult.csv
    sudo ./attack_ntmp_sampled >> error_no_mult.csv
    sudo ./attack_ntmp_benchmark >> error_no_mult.csv
    sudo ./attack_memset_sampled >> error_no_mult.csv
    sudo ./attack_memset_benchmark >> error_no_mult.csv
    sudo ./no_flushing_sampled >> error_no_mult.csv
    sudo ./no_flushing_benchmark >> error_no_mult.csv
    sudo ./random_sampled >> error_no_mult.csv
    sudo ./random_benchmark >> error_no_mult.csv
    sudo ./rowhammer_test_sampled >> error_no_mult.csv
    sudo ./rowhammer_test_benchmark >> error_no_mult.csv
    sudo ./double_sided_rowhammer_sampled >> error_no_mult.csv
    sudo ./double_sided_rowhammer_benchmark >> error_no_mult.csv
    NUM_RUNS=$(( $NUM_RUNS - 1 ))
done

sudo make clean























    
