# Used as resources:
#	https://bitbucket.org/icl/papi/wiki/PAPI-Overview.md
#	Makefiles from src/ctests directory

TESTS_PERF = attack_perf no_flushing_perf random_perf rowhammer_test4_perf rowhammer_test8_perf rowhammer_test9_perf rowhammer_test10_perf rowhammer_test20_perf double_sided_rowhammer_perf attack_ntmp_perf stream_perf
LIBPAPI = ../../src/libpapi.a
TESTS_PAPI = attack_papi no_flushing_papi random_papi rowhammer_test4_papi rowhammer_test8_papi rowhammer_test9_papi rowhammer_test10_papi rowhammer_test20_papi double_sided_rowhammer_papi attack_memset_papi attack_ntmp_papi stream_papi

all : $(TESTSPERF) $(TESTS_PAPI)

all_perf : $(TESTS_PERF)

attack_perf : attack_perf.c
	gcc -g -I. $^ -o $@

attack_papi : attack_papi.c handle_error.o
	gcc -g -lpthread -I. $^ $(LIBPAPI) -o $@

attack_ntmp_perf : attack_ntmp_perf.c
	gcc -g -I. $^ -o $@

attack_ntmp_papi : attack_ntmp_papi.c handle_error.o
	gcc -g -lpthread -I. $^ $(LIBPAPI) -o $@

no_flushing_perf : no_flushing_perf.c
	gcc -g -I. $^ -o $@

no_flushing_papi : no_flushing_papi.c handle_error.o
	gcc -g -lpthread -I. $^ $(LIBPAPI) -o $@

random_perf : random_perf.c
	gcc -g -I. $^ -o $@

random_papi : random_papi.c handle_error.o
	gcc -g -lpthread -I. $^ $(LIBPAPI) -o $@

rowhammer_test4_perf: rowhammer_test4_perf.cc
	g++ -g -O2 -I. rowhammer_test4_perf.cc -o rowhammer_test4_perf

rowhammer_test4_papi: rowhammer_test4_papi.cc handle_error.o
	g++ -g -O2 -lpthread -I. rowhammer_test4_papi.cc $(LIBPAPI) -o rowhammer_test4_papi

rowhammer_test8_perf: rowhammer_test8_perf.cc
	g++ -g -O2 -I. rowhammer_test8_perf.cc -o rowhammer_test8_perf

rowhammer_test8_papi: rowhammer_test8_papi.cc handle_error.o
	g++ -g -O2 -lpthread -I. rowhammer_test8_papi.cc $(LIBPAPI) -o rowhammer_test8_papi

rowhammer_test9_perf: rowhammer_test9_perf.cc
	g++ -g -O2 -I. rowhammer_test9_perf.cc -o rowhammer_test9_perf

rowhammer_test9_papi: rowhammer_test9_papi.cc handle_error.o
	g++ -g -O2 -lpthread -I. rowhammer_test9_papi.cc $(LIBPAPI) -o rowhammer_test9_papi

rowhammer_test10_perf: rowhammer_test10_perf.cc
	g++ -g -O2 -I. rowhammer_test10_perf.cc -o rowhammer_test10_perf

rowhammer_test10_papi: rowhammer_test10_papi.cc handle_error.o
	g++ -g -O2 -lpthread -I. rowhammer_test10_papi.cc $(LIBPAPI) -o rowhammer_test10_papi

rowhammer_test20_perf: rowhammer_test20_perf.cc
	g++ -g -O2 -I. rowhammer_test20_perf.cc -o rowhammer_test20_perf

rowhammer_test20_papi: rowhammer_test20_papi.cc handle_error.o
	g++ -g -O2 -lpthread -I. rowhammer_test20_papi.cc $(LIBPAPI) -o rowhammer_test20_papi

double_sided_rowhammer_perf: double_sided_rowhammer_perf.cc
	g++ -g -O2 -std=c++11 -I. double_sided_rowhammer_perf.cc -o double_sided_rowhammer_perf

double_sided_rowhammer_papi: double_sided_rowhammer_papi.cc handle_error.o
	g++ -g -O2 -std=c++11 -lpthread -I. double_sided_rowhammer_papi.cc $(LIBPAPI) -o double_sided_rowhammer_papi

stream_perf: stream_perf.c
	gcc -O $^ -o $@

stream_papi: stream_papi.c handle_error.o
	gcc -g -lpthread -I. -O $^ $(LIBPAPI) -o $@

clean :
	rm $(TESTS) $(TESTS_PAPI) handle_error.o
