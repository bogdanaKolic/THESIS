#!/bin/sh

NUM_RUNS=50

sudo make

while [ $NUM_RUNS -gt 0 ]
do
    sudo ./attack_sampled >> lenovo_load.csv
    sudo ./attack_benchmark >> lenovo_load.csv
    sudo ./attack_ntmp_sampled >> lenovo_load.csv
    sudo ./attack_ntmp_benchmark >> lenovo_load.csv
    sudo ./no_flushing_sampled >> lenovo_load.csv
    sudo ./no_flushing_benchmark >> lenovo_load.csv
    sudo ./random_sampled >> lenovo_load.csv
    sudo ./random_benchmark >> lenovo_load.csv
    sudo ./rowhammer_test_sampled >> lenovo_load.csv
    sudo ./rowhammer_test_benchmark >> lenovo_load.csv
    sudo ./double_sided_rowhammer_sampled >> lenovo_load.csv
    sudo ./double_sided_rowhammer_benchmark >> lenovo_load.csv
    sudo ./stream_sampled >> lenovo_load.csv
    sudo ./stream_benchmark >> lenovo_load.csv
    NUM_RUNS=$(( $NUM_RUNS - 1 ))
done

sudo make clean























    
