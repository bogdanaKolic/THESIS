# Used as resources:
#	https://bitbucket.org/icl/papi/wiki/PAPI-Overview.md
#	Makefiles from src/ctests directory
#	make.sh from the rowhammer-test repository

TESTS = attack rh4_test rh8_test rh9_test rh10_test rh20_test double_sided_rowhammer attack_ntmp
TESTS_PERF = attack_perf rowhammer_test4_perf rowhammer_test8_perf rowhammer_test9_perf rowhammer_test10_perf rowhammer_test20_perf double_sided_rowhammer_perf attack_ntmp_perf


all : $(TESTS) $(TESTS_PERF)

all_perf : $(TESTS_PERF)

attack_perf : attack_perf.c
	gcc -g -I. $^ -o $@

attack : attack.c
	gcc -g -I. $^ -o $@

attack_ntmp_perf : attack_ntmp_perf.c
	gcc -g -I. $^ -o $@

attack_ntmp : attack_ntmp.c
	gcc -g -I. $^ -o $@

rowhammer_test4_perf: rowhammer_test4_perf.cc
	g++ -g -O2 -I. rowhammer_test4_perf.cc -o rowhammer_test4_perf

rh4_test: rh4_test.cc
	g++ -g -O2 -I. rh4_test.cc -o rh4_test

rowhammer_test8_perf: rowhammer_test8_perf.cc
	g++ -g -O2 -I. rowhammer_test8_perf.cc -o rowhammer_test8_perf

rh8_test: rh8_test.cc
	g++ -g -O2 -I. rh8_test.cc -o rh8_test

rowhammer_test9_perf: rowhammer_test9_perf.cc
	g++ -g -O2 -I. rowhammer_test9_perf.cc -o rowhammer_test9_perf

rh9_test: rh9_test.cc
	g++ -g -O2 -I. rh9_test.cc -o rh9_test

rowhammer_test10_perf: rowhammer_test10_perf.cc
	g++ -g -O2 -I. rowhammer_test10_perf.cc -o rowhammer_test10_perf

rh10_test: rh10_test.cc
	g++ -g -O2 -I. rh10_test.cc -o rh10_test

rowhammer_test20_perf: rowhammer_test20_perf.cc
	g++ -g -O2 -I. rowhammer_test20_perf.cc -o rowhammer_test20_perf

rh20_test: rh20_test.cc
	g++ -g -O2 -I. rh20_test.cc -o rh20_test

double_sided_rowhammer_perf: double_sided_rowhammer_perf.cc
	g++ -g -O2 -std=c++11 -I. double_sided_rowhammer_perf.cc -o double_sided_rowhammer_perf

double_sided_rowhammer: double_sided_rowhammer.cc
	g++ -g -O2 -std=c++11 -I. double_sided_rowhammer.cc -o double_sided_rowhammer

clean :
	rm $(TESTS) $(TESTS_PERF)
