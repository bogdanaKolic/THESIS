# Used as resources:
#	https://bitbucket.org/icl/papi/wiki/PAPI-Overview.md
#	Makefiles from src/ctests directory

TESTS = attack no_flushing random rowhammer_test double_sided_rowhammer attack_memset attack_ntmp

all : $(TESTS)

attack : attack.c
	gcc -g -I. $^ -o $@

attack_memset : attack_memset.c
	gcc -g -I. $^ -o $@

attack_ntmp : attack_ntmp.c
	gcc -g -I. $^ -o $@

no_flushing : no_flushing.c
	gcc -g -I. $^ -o $@

random : random.c
	gcc -g -I. $^ -o $@

rowhammer_test: rowhammer_test.cc
	g++ -g -O2 -I. rowhammer_test.cc -o rowhammer_test

double_sided_rowhammer: double_sided_rowhammer.cc
	g++ -g -O2 -std=c++11 -I. double_sided_rowhammer.cc -o double_sided_rowhammer

clean :
	rm $(TESTS)
