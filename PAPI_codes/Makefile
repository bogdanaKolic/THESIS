# Used as resources:
#	https://bitbucket.org/icl/papi/wiki/PAPI-Overview.md
#	Makefiles from src/ctests directory

DIR = ../src
LIBPAPI = ../src/libpapi.a
TESTS = hammer attack_program

all : $(TESTS)


handle_error.o : handle_error.c
	gcc -g -I$(DIR) -L$(LIBPAPI) -c handle_error.c -lpapi

hammer : hammer.c handle_error.o
	gcc -g -I$(DIR) $^ -O0 $(LIBPAPI) -o $@

attack_program : attack_program.c handle_error.o
	gcc -g -I$(DIR) $^ -O0 $(LIBPAPI) -o $@

clean :
	rm $(TESTS) handle_error.o
