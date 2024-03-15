TESTS = attack rh4_test rh8_test rh9_test rh10_test rh20_test double_sided_rowhammer attack_ntmp

all : $(TESTS)

attack : attack.c
	gcc -g -I. $^ -o $@

attack_ntmp : attack_ntmp.c
	gcc -g -I. $^ -o $@

rh4_test: rh4_test.cc
	g++ -g -O2 -I. rh4_test.cc -o rh4_test

rh8_test: rh8_test.cc
	g++ -g -O2 -I. rh8_test.cc -o rh8_test

rh9_test: rh9_test.cc
	g++ -g -O2 -I. rh9_test.cc -o rh9_test

rh10_test: rh10_test.cc
	g++ -g -O2 -I. rh10_test.cc -o rh10_test

rh20_test: rh20_test.cc
	g++ -g -O2 -I. rh20_test.cc -o rh20_test

double_sided_rowhammer: double_sided_rowhammer.cc
	g++ -g -O2 -std=c++11 -I. double_sided_rowhammer.cc -o double_sided_rowhammer

clean :
	rm $(TESTS)
