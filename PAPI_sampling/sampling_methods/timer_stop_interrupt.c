/* A program where the main computations are periodically 
* interrupted using signals and a virtual interval timer, 
* and where HPCs are stopped/restarted at each interruption.
*
**********************************************************
* Resources:
*   https://stackoverflow.com/a/65571169 
*   https://linux.die.net/man/2/setitimer */

#include <stdlib.h>
#include <stdio.h>
#include <papi.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <assert.h>

#define PERIOD 1000 // in usec
#define EVENT "INSTRUCTIONS"

static sigset_t block ;
void timer_handler ();
int retval = 0;
int EventSet = PAPI_NULL;
unsigned int native = 0x0;
long_long values[1];
long_long sum = 0;

static void init( ) __attribute__((constructor));
void init(){

    /* Initialize the library */
    retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
        printf("PAPI library init error!\n");
        exit(1);
    }

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

    /* Read the counters by stopping them */
    if (PAPI_stop(EventSet, values) != PAPI_OK){
            printf("stop error\n");
            handle_error(1);
        }

    /* Print the result */
    printf("result (%s): %lld\n", EVENT, values[0]);

    /* Add it to the count */
    sum += values[0];

    /* Restart the counters */
    if (PAPI_start(EventSet) != PAPI_OK){
            printf("start error\n");
            handle_error(1);
        }
}

int main()
{
    /* Start counting */
    if (PAPI_start(EventSet) != PAPI_OK){
        printf("start error");
        handle_error(1);
    }

    /* Do some computation here */ 
    int j = 0;
    for(int i = 0; i<10000000; i++){
        j = j+i;
    }

    /* Stop counting */
    if (PAPI_stop(EventSet, values) != PAPI_OK)
        handle_error(1);
    
    printf("end result (%s): %lld\n", EVENT, values[0]);
    sum += values[0];

    printf("total (%s) : %lld\n", EVENT, sum);
    /* Exit successfully */
    printf("Success!\n");
    exit(0);
}

