/* This program is used to test whether putting a timer
* in a separate thread improves the accuracy when reading
* the HPCs periodically using interval timer interrupts and
* signal blocking , and uses ITIMER_REAL to test whether 
* the interruptions are handled at equal intervals 
*
*********************************************************************************************************
* Resources:
*   https://stackoverflow.com/a/65571169 
*   https://linux.die.net/man/2/setitimer 
*   https://stackoverflow.com/questions/47287644/profiling-cache-misses-for-separate-pthread-using-papi 
*   https://github.com/google/rowhammer-test 
*   https://linux.die.net/man/3/papi_attach
*   https://stackoverflow.com/questions/20728773/multithreaded-server-signal-handling-posix/20728819#20728819
*   https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_sigmask.html
*   https://icl.utk.edu/projects/papi/files/documentation/PAPI_USER_GUIDE_23.htm#WHAT_IS_MULTIPLEXING 
*   https://bitbucket.org/icl/papi/wiki/PAPI-Overview.md
*   https://www.mankier.com/3/PAPI_set_multiplex */

#include <stdlib.h>
#include <stdio.h>
#include <papi.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

#define BUFFER_SIZE (1 << 18)
#define BUFFER_SIZE_MASK (BUFFER_SIZE - 1)
#define RH_THRESHOLD (20000)
#define LF "\n"

#define FLUSH(addr) asm volatile("CLFLUSH (%[a])"::[a] "r" (addr):)

#define PERIOD 1000

#define L3M "ix86arch::LLC_MISSES"
#define L2H "L2_RQSTS:DEMAND_DATA_RD_HIT"
#define L1DH "MEM_LOAD_RETIRED:L1_HIT"
#define STALL "UOPS_RETIRED:STALL_CYCLES"

static sigset_t block ;
void timer_handler ();
int retval = 0;
int EventSet = PAPI_NULL;
unsigned int native = 0x0;
long_long values[4];
FILE *f;

struct sigaction act={0};
struct timeval interval;
struct itimerval period;

#ifndef FILENAME
#define FILENAME "attack_load.dat"
#endif // FILENAME

struct timeval start_time_;



double get_diff() {
    struct timeval end_time;
    int rc = gettimeofday(&end_time, NULL);
    assert(rc == 0);
    return (end_time.tv_sec - start_time_.tv_sec
            + (double) (end_time.tv_usec - start_time_.tv_usec) / 1e6);
  }

void *thread_func(void * args){
    
    /* Get the new thread id */
    long unsigned tid;
    tid = syscall(SYS_gettid);
    

    /* Block the signal handling in this thread */
    pthread_sigmask(SIG_BLOCK, &block, NULL);
    
    int retval;

    /* Attach the EventSet to the new thread */
    retval = PAPI_attach(EventSet, tid);
    if(retval != PAPI_OK) {
        printf("error attach\n");
        handle_error(retval);
    }
    
    /* Start counting the time */
    int rc = gettimeofday(&start_time_, NULL);
    assert(rc == 0);

    setitimer(ITIMER_REAL,&period,NULL);

    /* Start the counters for the new thread */
    if (PAPI_start(EventSet) != PAPI_OK){
        printf("error start");
        handle_error(1);
    }

    /* INSERT THE PROGRAM HERE */ 
    asm volatile ("start_program:");
    for(int i=0; i< 1000; i++){
        hammer();
    }
    asm volatile ("end_program:\nnop");
    

    /* Stop all counters */
    if (PAPI_stop(EventSet, values) != PAPI_OK){
        printf("null error when stopping in thread\n");
        handle_error(1);
}


    printf("attack, %lld %lld %lld %lld\n", values[0], values[1], values[2], values[3]);

    /* Exit successfully */
    exit(0);


}



void attack(const uint32_t nb_loops, const uintptr_t addr1, const uintptr_t addr2)
{
    asm volatile(
        "MOV %[nb_loops], %%ecx  \n"
        "JMP hammer_loop_asm     \n"
        "hammer_loop_asm:        \n"
        "  MOV (%[addr1]), %%edx \n"
        "  MOV (%[addr2]), %%edx \n"
        "  CLFLUSH (%[addr1])    \n"
        "  CLFLUSH (%[addr2])    \n"
	      "  LOOP hammer_loop_asm  \n"
        :: [nb_loops] "r" (nb_loops),
	   [addr1] "r" (addr1),
           [addr2] "r" (addr2)
        : "%ecx", "%edx", "memory"
        );

}

typedef union {
  uintptr_t addr;
  struct {
    uintptr_t buffer: 6;
    uintptr_t column: 7;
    uintptr_t bank_group: 2;
    uintptr_t bank: 2;
    uintptr_t row: 15;
};
} addr_t;

void hammer() {
  uint32_t buffer[(5<<(6+7+2+2))/sizeof(uint32_t)]; // 5 rows
  addr_t agg1 = {.addr = (uintptr_t) &(buffer[0])};
  addr_t vict = {.addr = agg1.addr};
  addr_t agg2 = {.addr = agg1.addr};
  vict.row += 1;
  agg2.row += 2;
  // vict.row += 2;
  memset(buffer, 0xFF, sizeof(buffer));
  // printf("%lld\n", (5<<(6+7+2+2))/sizeof(uint32_t)); gives 163840
  /* printf("agg1:%xh[%d|%d|%xh|%xh]"LF, agg1.addr, agg1.bank_group, agg1.bank, agg1.row, agg1.column);
  printf("agg2:%xh[%d|%d|%xh|%xh]"LF, agg2.addr, agg2.bank_group, agg2.bank, agg2.row, agg2.column);
  printf("vict:%xh[%d|%d|%xh|%xh] = %08Xh", vict.addr, vict.bank_group, vict.bank, vict.row, 
                                            vict.column, *((int*)vict.addr)); */

  FLUSH(vict.addr);
  attack((RH_THRESHOLD+1)/2, agg1.addr, agg2.addr);
  FLUSH(vict.addr);
  /* printf("vict:%xh[%d|%d|%xh|%xh] = %08Xh", vict.addr, vict.bank_group, vict.bank, vict.row,
                                            vict.column, *((int*)vict.addr)); */
}


static void init( ) __attribute__((constructor));
void init(){

    /* Initialize the library */
    retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
        printf("PAPI library init error!\n");
        exit(1);
    }

    /* Thread init */
    retval = PAPI_thread_init(pthread_self);
    if(retval != PAPI_OK){
        printf("thread error\n");
        handle_error(retval);
    }

    /* Create an EventSet */
    retval = PAPI_create_eventset(&EventSet);
    if (retval != PAPI_OK){
     printf("create_eventset error");
     handle_error(retval);
    }
    
    /* Find the code for the native event */
    retval = PAPI_event_name_to_code(L3M, &native);
    if (retval != PAPI_OK){
        printf("event error");
        handle_error(retval);
    }

    /* Add it to the eventset */
    retval = PAPI_add_event(EventSet, native);
    if (retval != PAPI_OK){
        printf("add event error");
        handle_error(retval);
    }

    /* Find the code for the native event */
    retval = PAPI_event_name_to_code(L2H, &native);
    if (retval != PAPI_OK){
        printf("event error");
        handle_error(retval);
    }

    /* Add it to the eventset */
    retval = PAPI_add_event(EventSet, native);
    if (retval != PAPI_OK){
        printf("add event error");
        handle_error(retval);
    }

    /* Find the code for the native event */
    retval = PAPI_event_name_to_code(L1DH, &native);
    if (retval != PAPI_OK){
        printf("event error");
        handle_error(retval);
    }

    /* Add it to the eventset */
    retval = PAPI_add_event(EventSet, native);
    if (retval != PAPI_OK){
        printf("add event error");
        handle_error(retval);
    }

    /* Find the code for the native event */
    retval = PAPI_event_name_to_code(STALL, &native);
    if (retval != PAPI_OK){
        printf("event error");
        handle_error(retval);
    }

    /* Add it to the eventset */
    retval = PAPI_add_event(EventSet, native);
    if (retval != PAPI_OK){
        printf("add event error");
        handle_error(retval);
    }

    /* File for the output */
    f = fopen(FILENAME, "w");

    /* Initialize the timer */
    sigemptyset(&block);
    sigaddset(&block,SIGALRM);

    act.sa_handler=timer_handler;
    assert(sigaction(SIGALRM,&act,NULL)==0);

    interval.tv_sec=0;
    interval.tv_usec=PERIOD;
    period.it_interval=interval;
    period.it_value=interval;


    
}


void timer_handler(int sig){
    /* The function called at the interrupt */

    /* Measure the time taken */
    double diff = get_diff();

   /* Read all counters */
    if (PAPI_read(EventSet, values) != PAPI_OK){
        printf("read error\n");
        handle_error(1);
    }

    /* Reset the counters */
    if (PAPI_reset(EventSet) != PAPI_OK){
        printf("reset error\n");
        handle_error(1);
    }

    /* Print the results */
    // printf("result (%s): %lld\n", EVENT, values[0]);
    fprintf(f, "attack %lf %lld %lld %lld %lld\n", diff, values[0], values[1], values[2], values[3]);

}

int main()
{

    /* Create a new thread */
    pthread_t my_thread;
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    pthread_create( &my_thread, &attr, &thread_func, NULL);
    

    while(1);
    /* Stop all counters */
    if (PAPI_stop(EventSet, values) != PAPI_OK){
        printf("error stop\n");
        handle_error(1);
}

    /* Exit successfully */
    //exit(0);
}

