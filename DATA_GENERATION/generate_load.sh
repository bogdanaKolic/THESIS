#!/bin/bash

laptop="asus"
load=4
LIBPAPI = ../../src/libpapi.a

# compile
make handle_error.o
gcc -g -lpthread -I. attack_papi.c handle_error.o $(LIBPAPI) -DLAPTOP=$(laptop) -DLOAD=$(load) -o attack_papi
gcc -g -lpthread -I. attack_ntmp_papi.c handle_error.o $(LIBPAPI) -DLAPTOP=$(laptop) -DLOAD=$(load) -o attack_ntmp_papi
gcc -g -lpthread -I. no_flushing_papi.c handle_error.o $(LIBPAPI) -DLAPTOP=$(laptop) -DLOAD=$(load) -o no_flushing_papi
gcc -g -lpthread -I. random_papi.c handle_error.o $(LIBPAPI) -DLAPTOP=$(laptop) -DLOAD=$(load) -o random_papi
g++ -g -O2 -lpthread -I. rowhammer_test4_papi.cc $(LIBPAPI) -DLAPTOP=$(laptop) -DLOAD=$(load) -o rowhammer_test4_papi
g++ -g -O2 -lpthread -I. rowhammer_test8_papi.cc $(LIBPAPI) -DLAPTOP=$(laptop) -DLOAD=$(load) -o rowhammer_test8_papi
g++ -g -O2 -lpthread -I. rowhammer_test9_papi.cc $(LIBPAPI) -DLAPTOP=$(laptop) -DLOAD=$(load) -o rowhammer_test9_papi
g++ -g -O2 -lpthread -I. rowhammer_test10_papi.cc $(LIBPAPI) -DLAPTOP=$(laptop) -DLOAD=$(load) -o rowhammer_test10_papi
g++ -g -O2 -lpthread -I. rowhammer_test20_papi.cc $(LIBPAPI) -DLAPTOP=$(laptop) -DLOAD=$(load) -o rowhammer_test20_papi
g++ -g -O2 -std=c++11 -lpthread -I. double_sided_rowhammer_papi.cc $(LIBPAPI) -DLAPTOP=$(laptop) -DLOAD=$(load) -o double_sided_rowhammer_papi
gcc -g -lpthread -I. -O stream_papi.c handle_error.o $(LIBPAPI) -DLAPTOP=$(laptop) -DLOAD=$(load) -o stream_papi

#run
./attack_papi
./attack_ntmp_papi
./no_flushing_papi
./random_papi
./rowhammer_test4_papi
./rowhammer_test8_papi
./rowhammer_test9_papi
./rowhammer_test10_papi
./rowhammer_test20_papi
./double_sided_rowhammer_papi
./stream_papi

make clean
