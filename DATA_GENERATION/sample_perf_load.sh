#!/bin/bash

laptop="dell"

# compile
make all_perf

#run
perf stat -e cpu_core/event=0x2e,umask=0x41/u,cpu_core/event=0x24,umask=0xc1/u,cpu_core/event=0xd1,umask=0x01/u,cpu_core/event=0xc2,umask=0x02,inv=1,cmask=0x01/u -I 1 -o "attack_load_dell.dat" ./attack_perf
perf stat -e cpu_core/event=0x2e,umask=0x41/u,cpu_core/event=0x24,umask=0xc1/u,cpu_core/event=0xd1,umask=0x01/u,cpu_core/event=0xc2,umask=0x02,inv=1,cmask=0x01/u -I 1 -o "attack_ntmp_load_dell.dat" ./attack_ntmp_perf
perf stat -e cpu_core/event=0x2e,umask=0x41/u,cpu_core/event=0x24,umask=0xc1/u,cpu_core/event=0xd1,umask=0x01/u,cpu_core/event=0xc2,umask=0x02,inv=1,cmask=0x01/u -I 1 -o "no_flushing_load_dell.dat" ./no_flushing_perf
perf stat -e cpu_core/event=0x2e,umask=0x41/u,cpu_core/event=0x24,umask=0xc1/u,cpu_core/event=0xd1,umask=0x01/u,cpu_core/event=0xc2,umask=0x02,inv=1,cmask=0x01/u -I 1 -o "random_load_dell.dat" ./random_perf
perf stat -e cpu_core/event=0x2e,umask=0x41/u,cpu_core/event=0x24,umask=0xc1/u,cpu_core/event=0xd1,umask=0x01/u,cpu_core/event=0xc2,umask=0x02,inv=1,cmask=0x01/u -I 1 -o "rh4_load_dell.dat" ./rowhammer_test4_perf
perf stat -e cpu_core/event=0x2e,umask=0x41/u,cpu_core/event=0x24,umask=0xc1/u,cpu_core/event=0xd1,umask=0x01/u,cpu_core/event=0xc2,umask=0x02,inv=1,cmask=0x01/u -I 1 -o "rh8_load_dell.dat" ./rowhammer_test8_perf
perf stat -e cpu_core/event=0x2e,umask=0x41/u,cpu_core/event=0x24,umask=0xc1/u,cpu_core/event=0xd1,umask=0x01/u,cpu_core/event=0xc2,umask=0x02,inv=1,cmask=0x01/u -I 1 -o "rh9_load_dell.dat" ./rowhammer_test9_perf
perf stat -e cpu_core/event=0x2e,umask=0x41/u,cpu_core/event=0x24,umask=0xc1/u,cpu_core/event=0xd1,umask=0x01/u,cpu_core/event=0xc2,umask=0x02,inv=1,cmask=0x01/u -I 1 -o "rh10_load_dell.dat" ./rowhammer_test10_perf
perf stat -e cpu_core/event=0x2e,umask=0x41/u,cpu_core/event=0x24,umask=0xc1/u,cpu_core/event=0xd1,umask=0x01/u,cpu_core/event=0xc2,umask=0x02,inv=1,cmask=0x01/u -I 1 -o "rh20_load_dell.dat" ./rowhammer_test20_perf
perf stat -e cpu_core/event=0x2e,umask=0x41/u,cpu_core/event=0x24,umask=0xc1/u,cpu_core/event=0xd1,umask=0x01/u,cpu_core/event=0xc2,umask=0x02,inv=1,cmask=0x01/u -I 1 -o "double_sided_load_dell.dat" ./double_sided_rowhammer_perf
perf stat -e cpu_core/event=0x2e,umask=0x41/u,cpu_core/event=0x24,umask=0xc1/u,cpu_core/event=0xd1,umask=0x01/u,cpu_core/event=0xc2,umask=0x02,inv=1,cmask=0x01/u -I 1 -o "stream_load_dell.dat" ./stream_perf

make clean
