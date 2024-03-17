/* This program is used to test whether putting a timer
* in a separate thread improves the accuracy when reading
* the HPCs periodically using interval timer interrupts
*
*********************************************************************************************************
* Resources:
*   https://stackoverflow.com/a/65571169 
*   https://linux.die.net/man/2/setitimer 
*   https://linux.die.net/man/3/papi_attach
*   https://stackoverflow.com/questions/47287644/profiling-cache-misses-for-separate-pthread-using-papi */

#include <stdlib.h>
#include <stdio.h>
#include <papi.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <sys/syscall.h>

#define PERIOD 1000
#define EVENT "INSTRUCTIONS"

static sigset_t block ;
void timer_handler ();
int retval = 0;
int EventSet = PAPI_NULL, EventSetMain = PAPI_NULL;
unsigned int native = 0x0;
long_long values[1], values_main[1];

void *thread_func(void * args){

    /* Print the new thread id */
    long unsigned tid;
    tid = syscall(SYS_gettid);
    printf("new thread id: %ld\n", tid);
    
    
    int retval;

    /* Attach the EventSet to the new thread */
    retval = PAPI_attach(EventSet, tid);
    if(retval != PAPI_OK) {
        printf("error attach\n");
        handle_error(retval);
    }
    
    /* Start the counters for the new thread */
    if (PAPI_start(EventSet) != PAPI_OK){
        printf("error start");
        handle_error(1);
    }

   /* Do some computation here */ 
    int j = 0;
    for(int i = 0; i<10000000; i++){
        j = j+i;
    }

    /* Stop all counters */
    if (PAPI_stop(EventSet, values) != PAPI_OK)
        handle_error(1);
    if (PAPI_stop(EventSetMain, values_main) != PAPI_OK)
        handle_error(1);
    
    printf("Stopped in the second thread\n");
    printf("end result: %lld\n", values[0]);
    printf("end result main: %lld\n", values_main[0]);

    /* Exit successfully */
    printf("Success!\n");
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
    retval = PAPI_event_name_to_code(EVENT, &native);
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
    
    /* Create an EventSetMain for the main thread */
    retval = PAPI_create_eventset(&EventSetMain);
    if (retval != PAPI_OK){
     printf("create_eventset error");
     handle_error(retval);
    }

    /* Add the event to the new eventset */
    retval = PAPI_add_event(EventSetMain, native);
    if (retval != PAPI_OK){
    printf("add event error");
    handle_error(retval);
    }
    
    /* Initialize the timer */
    sigemptyset(&block);
    sigaddset(&block,SIGVTALRM);

    struct sigaction act={0};
    struct timeval interval;
    struct itimerval period;

    act.sa_handler=timer_handler;
    assert(sigaction(SIGVTALRM,&act,NULL)==0);

    interval.tv_sec=0;
    interval.tv_usec=PERIOD;
    period.it_interval=interval;
    period.it_value=interval;
    setitimer(ITIMER_VIRTUAL,&period,NULL);

    
}


void timer_handler(int sig){
    /* The function called at the interrupt */

    /* Read all counters */
    if (PAPI_read(EventSetMain, values_main) != PAPI_OK){
        printf("read error\n");
        handle_error(1);
    }
    if (PAPI_read(EventSet, values) != PAPI_OK){
        printf("read error\n");
        handle_error(1);
    }

    /* Print the results */
    printf("result: %lld\n", values[0]);
    printf("result main thread: %lld\n", values_main[0]);


}

int main()
{
    /* Print the main thread id */
    long unsigned tid;
    tid = syscall(SYS_gettid);
    printf("main tid: %ld\n", tid);

    /* Attach the EventSetMain to the main thread */
    retval = PAPI_attach(EventSetMain, tid);
    if(retval != PAPI_OK) {
        printf("error attach\n");
        handle_error(retval);
    }
    

    /* Create a new thread */
    pthread_t my_thread;
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    pthread_create( &my_thread, &attr, &thread_func, NULL);
    
    /* Start counting the events in the main thread */
    PAPI_start(EventSetMain);

    /* Do some computation here */ 

   /* for(int i=0; i< 100000000; i++){
        //printf("%d\n", i);
    }*/
    int j = 0, s;
    while(1){s=j++;}; 
    //pthread_join(my_thread, NULL);

    /* Stop all counters */
    if (PAPI_stop(EventSet, values) != PAPI_OK)
        handle_error(1);
    if (PAPI_stop(EventSetMain, values_main) != PAPI_OK)
        handle_error(1);
    
    printf("Stopped in the main thread\n");
    printf("end result: %lld\n", values[0]);
    printf("end result main: %lld\n", values_main[0]);

    /* Exit successfully */
    printf("Success!\n");
    exit(0);
}

