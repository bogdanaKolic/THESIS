#!/bin/sh

NUM_RUNS=50

sudo make

while [ $NUM_RUNS -gt 0 ]
do
    sudo ./attack_sampled >> asus_load.csv
    sudo ./attack_benchmark >> asus_load.csv
    sudo ./attack_ntmp_sampled >> asus_load.csv
    sudo ./attack_ntmp_benchmark >> asus_load.csv
    sudo ./no_flushing_sampled >> asus_load.csv
    sudo ./no_flushing_benchmark >> asus_load.csv
    sudo ./random_sampled >> asus_load.csv
    sudo ./random_benchmark >> asus_load.csv
    sudo ./rowhammer_test_sampled >> asus_load.csv
    sudo ./rowhammer_test_benchmark >> asus_load.csv
    sudo ./double_sided_rowhammer_sampled >> asus_load.csv
    sudo ./double_sided_rowhammer_benchmark >> asus_load.csv
    sudo ./stream_sampled >> asus_load.csv
    sudo ./stream_benchmark >> asus_load.csv
    NUM_RUNS=$(( $NUM_RUNS - 1 ))
done

sudo make clean























    
