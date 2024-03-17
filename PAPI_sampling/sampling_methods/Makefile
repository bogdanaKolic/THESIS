# Used as resources:
#	https://bitbucket.org/icl/papi/wiki/PAPI-Overview.md
#	Makefiles from src/ctests directory

LIBPAPI = ../../../src/libpapi.a
TESTS = add_native timer_interrupt total_count_benchmark timer_stop_interrupt timer_thread timer_thread_sigblock total_count_overflow_benchmark sampling_overflow

all : $(TESTS)

handle_error.o : handle_error.c
	gcc -g -I. -c handle_error.c

add_native : add_native.c handle_error.o
	gcc -g -I. $^ $(LIBPAPI) -o $@

timer_interrupt : timer_interrupt.c handle_error.o
	gcc -g -I. $^ $(LIBPAPI) -o $@

total_count_benchmark : total_count_benchmark.c handle_error.o
	gcc -g -I. $^ $(LIBPAPI) -o $@

total_count_overflow_benchmark : total_count_overflow_benchmark.c handle_error.o
	gcc -g -I. $^ $(LIBPAPI) -o $@

timer_stop_interrupt : timer_stop_interrupt.c handle_error.o
	gcc -g -I. $^ $(LIBPAPI) -o $@

timer_thread : timer_thread.c handle_error.o
	gcc -g -lpthread -I. $^ $(LIBPAPI) -o $@

timer_thread_sigblock : timer_thread_sigblock.c handle_error.o
	gcc -g -lpthread -I. $^ $(LIBPAPI) -o $@

sampling_overflow : sampling_overflow.c handle_error.o
	gcc -g -I. $^ $(LIBPAPI) -o $@

clean :
	rm $(TESTS) handle_error.o
