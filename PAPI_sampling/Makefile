# Used as resources:
#	https://bitbucket.org/icl/papi/wiki/PAPI-Overview.md
#	Makefiles from src/ctests directory
#	make.sh from the rowhammer-test repository

LIBPAPI = ../../src/libpapi.a
TESTS = total_count_benchmark timer_thread_sigblock attack_sampled attack_benchmark no_flushing_sampled no_flushing_benchmark random_sampled random_benchmark rowhammer_test_sampled rowhammer_test_benchmark double_sided_rowhammer_sampled double_sided_rowhammer_benchmark attack_ntmp_sampled attack_ntmp_benchmark stream_sampled stream_benchmark

all : $(TESTS)

handle_error.o : handle_error.c
	gcc -g -I. -c handle_error.c

total_count_benchmark : total_count_benchmark.c handle_error.o
	gcc -g -I. $^ $(LIBPAPI) -o $@

timer_thread_sigblock : timer_thread_sigblock.c handle_error.o
	gcc -g -lpthread -I. $^ $(LIBPAPI) -o $@

attack_sampled : attack_sampled.c handle_error.o
	gcc -g -lpthread -I. $^ $(LIBPAPI) -o $@

attack_memset_sampled : attack_memset_sampled.c handle_error.o
	gcc -g -lpthread -I. $^ $(LIBPAPI) -o $@

attack_ntmp_sampled : attack_ntmp_sampled.c handle_error.o
	gcc -g -lpthread -I. $^ $(LIBPAPI) -o $@

no_flushing_sampled : no_flushing_sampled.c handle_error.o
	gcc -g -lpthread -I. $^ $(LIBPAPI) -o $@

random_sampled : random_sampled.c handle_error.o
	gcc -g -lpthread -I. $^ $(LIBPAPI) -o $@

rowhammer_test_sampled: rowhammer_test_sampled.cc handle_error.o
	g++ -g -O2 -lpthread -I. rowhammer_test_sampled.cc $(LIBPAPI) -o rowhammer_test_sampled

double_sided_rowhammer_sampled: double_sided_rowhammer_sampled.cc handle_error.o
	g++ -g -O2 -std=c++11 -lpthread -I. double_sided_rowhammer_sampled.cc $(LIBPAPI) -o double_sided_rowhammer_sampled

attack_benchmark : attack_benchmark.c handle_error.o
	gcc -g -I. $^ $(LIBPAPI) -o $@

attack_memset_benchmark : attack_memset_benchmark.c handle_error.o
	gcc -g -I. $^ $(LIBPAPI) -o $@

attack_ntmp_benchmark : attack_ntmp_benchmark.c handle_error.o
	gcc -g -I. $^ $(LIBPAPI) -o $@

no_flushing_benchmark : no_flushing_benchmark.c handle_error.o
	gcc -g -I. $^ $(LIBPAPI) -o $@

random_benchmark : random_benchmark.c handle_error.o
	gcc -g -I. $^ $(LIBPAPI) -o $@

rowhammer_test_benchmark: rowhammer_test_benchmark.cc handle_error.o
	g++ -g -O2 -I. rowhammer_test_benchmark.cc $(LIBPAPI) -o rowhammer_test_benchmark

double_sided_rowhammer_benchmark: double_sided_rowhammer_benchmark.cc handle_error.o
	g++ -g -O2 -std=c++11 -I. double_sided_rowhammer_benchmark.cc $(LIBPAPI) -o double_sided_rowhammer_benchmark

stream_sampled: stream_sampled.c handle_error.o
	gcc -g -lpthread -I. -O $^ $(LIBPAPI) -o $@

stream_benchmark: stream_benchmark.c handle_error.o
	gcc -g -lpthread -I. -O $^ $(LIBPAPI) -o $@

clean :
	rm $(TESTS) handle_error.o
