#!/bin/sh

NUM_RUNS=50

sudo make

while [ $NUM_RUNS -gt 0 ]
do
    sudo ./attack_sampled >> error_asus_load.csv
    sudo ./attack_benchmark >> error_asus_load.csv
    sudo ./attack_ntmp_sampled >> error_asus_load.csv
    sudo ./attack_ntmp_benchmark >> error_asus_load.csv
    sudo ./no_flushing_sampled >> error_asus_load.csv
    sudo ./no_flushing_benchmark >> error_asus_load.csv
    sudo ./random_sampled >> error_asus_load.csv
    sudo ./random_benchmark >> error_asus_load.csv
    sudo ./rowhammer_test_sampled >> error_asus_load.csv
    sudo ./rowhammer_test_benchmark >> error_asus_load.csv
    sudo ./double_sided_rowhammer_sampled >> error_asus_load.csv
    sudo ./double_sided_rowhammer_benchmark >> error_asus_load.csv
    sudo ./stream_sampled >> error_asus_load.csv
    sudo ./stream_benchmark >> error_asus_load.csv
    NUM_RUNS=$(( $NUM_RUNS - 1 ))
done

sudo make clean























    
