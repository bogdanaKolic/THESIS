# Used as resources:
#	https://bitbucket.org/icl/papi/wiki/PAPI-Overview.md
#	Makefiles from src/ctests directory

TESTS = attack_program attack no_flushing random attack_ntmp attack_memset

all : $(TESTS)

attack_program : attack_program.c
	gcc -g -I. $^ -O0 -o $@

attack : attack.c
	gcc -g -I. $^ -O0 -o $@

attack_ntmp : attack_ntmp.c
	gcc -g -I. $^ -O0 -o $@

attack_memset : attack_memset.c
	gcc -g -I. $^ -O0 -o $@

no_flushing : no_flushing.c
	gcc -g -I. $^ -O0 -o $@

random : random.c
	gcc -g -I. $^ -O0 -o $@

clean :
	rm $(TESTS)
