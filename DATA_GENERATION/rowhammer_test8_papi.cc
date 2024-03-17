/* Modified code from 
* https://github.com/google/rowhammer-test/tree/master */

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
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>

const size_t mem_size = 1 << 30;
const int toggles = 4000000;

char *g_mem;

#define PERIOD 1000

#define L3M "ix86arch::LLC_MISSES"
#define L2H "L2_RQSTS:DEMAND_DATA_RD_HIT"
#define L1DH "MEM_LOAD_RETIRED:L1_HIT"
#define STALL "UOPS_RETIRED:STALL_CYCLES"

void handle_error (int retval)
{
     printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
     exit(1);
}

static sigset_t block ;
void timer_handler ();
int retval = 0;
int EventSet = PAPI_NULL;
int native = 0;
long_long values[4];
FILE *f;

struct sigaction act={0};
struct timeval interval;
struct itimerval period;

#ifndef LAPTOP
#define LAPTOP "asus"
#endif //LAPTOP

#ifndef LOAD
#define LOAD 0
#define FILENAME "rh8_" LAPTOP ".csv"
#else
#define FILENAME "rh8_load_" LAPTOP ".csv"
#endif //LOAD

struct timeval start_time_;

double get_diff() {
    struct timeval end_time;
    int rc = gettimeofday(&end_time, NULL);
    assert(rc == 0);
    return (end_time.tv_sec - start_time_.tv_sec
            + (double) (end_time.tv_usec - start_time_.tv_usec) / 1e6);
  }

char *pick_addr() {
  size_t offset = (rand() << 12) % mem_size;
  return g_mem + offset;
}

static void toggle(int iterations, int addr_count) {
  for (int j = 0; j < iterations; j++) {
    uint64_t *addrs[addr_count];
    for (int a = 0; a < addr_count; a++)
      addrs[a] = (uint64_t *) pick_addr();

    uint64_t sum = 0;
    for (int i = 0; i < toggles; i++) {
      for (int a = 0; a < addr_count; a++)
        sum += *addrs[a] + 1;
      for (int a = 0; a < addr_count; a++)
        asm volatile("clflush (%0)" : : "r" (addrs[a]) : "memory");
    }

    // Sanity check.  We don't expect this to fail, because reading
    // these rows refreshes them.
    if (sum != 0) {
      printf("error: sum=%lx\n", sum);
      exit(1);
    }
  }

}

void main_prog() {
  g_mem = (char *) mmap(NULL, mem_size, PROT_READ | PROT_WRITE,
                        MAP_ANON | MAP_PRIVATE, -1, 0);
  assert(g_mem != MAP_FAILED);

  memset(g_mem, 0xff, mem_size);

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

    toggle(90, 8); //iterations, addr_count

    /* Stop all counters */
    if (PAPI_stop(EventSet, values) != PAPI_OK)
        handle_error(1);


    printf("rh8, %lld %lld %lld %lld\n", values[0], values[1], values[2], values[3]);

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

    
    
    main_prog();

    /* Exit successfully */
    exit(0);


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
    fprintf(f, "%s, %d, rh8, %lf, %lld, %lld, %lld, %lld, attack\n", LAPTOP, LOAD, diff, values[0], values[1], values[2], values[3]);

    
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



int main()
{

    /* Create a new thread */
    pthread_t my_thread;
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    pthread_create( &my_thread, &attr, &thread_func, NULL);
    

    while(1);

    /* Stop all counters */
    if (PAPI_stop(EventSet, values) != PAPI_OK)
        handle_error(1);
    

    /* Exit successfully */
    exit(0);
}

