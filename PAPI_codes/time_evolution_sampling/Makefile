# Used as resources:
#	https://bitbucket.org/icl/papi/wiki/PAPI-Overview.md
#	Makefiles from src/ctests directory

LIBPAPI = ../../../src/libpapi.a
TESTS = attack_sampled no_flushing_sampled random_sampled rowhammer_test_sampled double_sided_rowhammer_sampled attack_memset_sampled attack_ntmp_sampled

all : $(TESTS)

handle_error.o : handle_error.c
	gcc -g -I. -c handle_error.c

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

clean :
	rm $(TESTS) handle_error.o
