/* A program that performs sampling using by using PAPI_overflow function.
*
*********************************************************************************************************
* Sources:
*   https://github.com/google/rowhammer-test 
*   https://linux.die.net/man/3/papi_overflow */

#include <stdlib.h>
#include <stdio.h>
#include <papi.h>
#include <sys/time.h>
#include <assert.h>

#define PERIOD 416667 // the number of clock cycles
#define CYCLES "UNHALTED_CORE_CYCLES"
#define EVENT "INSTRUCTIONS"


long_long values[2]; // for storing the counters' values
long_long sum = 0; // total count of event occurrences

struct timeval start_time_;

double get_diff() {
    struct timeval end_time;
    int rc = gettimeofday(&end_time, NULL);
    assert(rc == 0);
    return ((end_time.tv_sec - start_time_.tv_sec) * 1e3
            + (end_time.tv_usec - start_time_.tv_usec) / 1e3);
  }

void handler(int EventSet, void *address, long_long overflow_vector, void *context){
    /* A function that is called at the overflow */

    /* Measure the interruption frequency */
    /*double diff = get_diff();
    gettimeofday(&start_time_, NULL);
    printf("time passed(msec): %lf\n", diff);

    /* Measurements with PAPI_stop/start */
    /*int retval = PAPI_stop(EventSet, values);
    if(retval != PAPI_OK){
    printf("read_error\n");
    handle_error(retval);
    }


    printf("result (%s): %lld cycles (%s):%lld\n", EVENT, values[0], CYCLES, values[1]);
    sum += values[0];


    if (PAPI_start(EventSet) != PAPI_OK)
        handle_error(1);

    /* Measurements with PAPI_read */
    int retval = PAPI_read(EventSet, values);
    if(retval != PAPI_OK){
        printf("read_error\n");
        handle_error(retval);
    }

    printf("result (%s): %lld cycles (%s):%lld\n", EVENT, values[0], CYCLES, values[1]);
}




int main()
{
    int retval, EventSet = PAPI_NULL;
    unsigned int native = 0x0, count = 0x0;

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
    if (retval != PAPI_OK) handle_error(retval);

    /* Find the code for the native event */
    retval = PAPI_event_name_to_code(CYCLES, &count);
    if (retval != PAPI_OK){
        printf("event error");
        handle_error(retval);
    }

    /* Add it to the eventset */
    retval = PAPI_add_event(EventSet, count);
    if (retval != PAPI_OK) handle_error(retval);
    
    /* Set overflow */
    retval = PAPI_overflow(EventSet, count, PERIOD, 0, handler);
    if( retval != PAPI_OK){
        printf("overflow error\n");
        handle_error(retval);
    }

    gettimeofday(&start_time_, NULL);

    /* Start counting */
    if (PAPI_start(EventSet) != PAPI_OK)
        handle_error(1);

    /* Do some computation here */ 
    int j = 0;
    for(int i = 0; i<10000000; i++){
        j = j+i;
    }

    /* Stop the counters */
    if (PAPI_stop(EventSet, values) != PAPI_OK)
        handle_error(1);

    printf("end result (%s): %lld cycles (%s):%lld\n", EVENT, values[0], CYCLES, values[1]);
    //sum += values[0];
    //printf("total count (%s) : %lld\n", EVENT, sum);

    /* Exit successfully */
    printf("Success!\n");
    exit(0);
}

