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
*   https://github.com/google/rowhammer-test for the get_diff() function
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

#define PERIOD 1000

#define INSTR "INST_RETIRED:ANY"
#define INSTR_LENOVO "INST_RETIRED:ANY_P"
#define MEMINSTR "MEM_INST_RETIRED:ANY"
#define L1DRA "PERF_COUNT_HW_CACHE_L1D:READ:ACCESS"

static sigset_t block ;
void timer_handler ();
int retval = 0;
int EventSet = PAPI_NULL;
unsigned int native = 0x0;
long_long values[3];
FILE *f;

struct sigaction act={0};
struct timeval interval;
struct itimerval period;

#ifndef FILENAME
#define FILENAME "code_sampled.dat"
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
    printf("new thread: %lld\n", tid);

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
    while(1){

    }
    

    /* Stop all counters */
    if (PAPI_stop(EventSet, values) != PAPI_OK)
        handle_error(1);


    printf("sampled_code, %lld, %lld, %lld, ", values[0], values[1], values[2]);

    /* Exit successfully */
    exit(0);


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
    if(retval != PAPI_OK)
        handle_error(retval);

    /* Create an EventSet */
    retval = PAPI_create_eventset(&EventSet);
    if (retval != PAPI_OK){
     printf("create_eventset error");
     handle_error(retval);
    }
    
    /* Find the code for the native event */
    retval = PAPI_event_name_to_code(INSTR_LENOVO, &native);
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
    retval = PAPI_event_name_to_code(MEMINSTR, &native);
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
    retval = PAPI_event_name_to_code(L1DRA, &native);
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
    f = fopen(FILENAME, "a");

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

    long unsigned tid;
    tid = syscall(SYS_gettid);
    printf("handler thread: %lld\n", tid);

   /* Read all counters */
    if (PAPI_read(EventSet, values) != PAPI_OK){
        printf("read error\n");
        handle_error(1);
    }

    /* Print the results */
    // printf("result (%s): %lld\n", EVENT, values[0]);
    fprintf(f, "sampled_code %lf %lld %lld %lld\n", diff, values[0], values[1], values[2]);


}

int main()
{

    long unsigned tid;
    tid = syscall(SYS_gettid);
    printf("main thread: %lld\n", tid);

    /* Create a new thread */
    pthread_t my_thread;
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    pthread_create( &my_thread, &attr, &thread_func, NULL);
    
    while(1){
        // do something
    }
    /* Stop all counters */
     if (PAPI_stop(EventSet, values) != PAPI_OK)
       handle_error(1);
    

    /* Exit successfully */
    exit(0);
}

