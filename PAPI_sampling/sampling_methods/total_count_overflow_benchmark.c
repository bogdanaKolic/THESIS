/* A program that serves as a benchmark for computing the
* total number of event occurrences without overflow interruptions.
*
*******************************************************************
* Sources:
*   https://stackoverflow.com/a/65571169 
*   https://linux.die.net/man/2/setitimer */

#include <stdlib.h>
#include <stdio.h>
#include <papi.h>

#define EVENT "INSTRUCTIONS"
#define CYCLES "UNHALTED_CORE_CYCLES"

int retval = 0;
int EventSet = PAPI_NULL;
unsigned int native = 0x0, count = 0x0;
long_long values[2];


int main()
{

    /* Initialize the library */
    retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
        printf("PAPI library init error!\n");
        exit(1);
    }

    /* Create an EventSet */
    retval = PAPI_create_eventset(&EventSet);
    if (retval != PAPI_OK){
     printf("create_eventset error\n");
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
    
    /* Find the code for the native event */
    retval = PAPI_event_name_to_code(CYCLES, &count);
    if (retval != PAPI_OK){
        printf("event error");
        handle_error(retval);
    }

    /* Add it to the eventset */
    retval = PAPI_add_event(EventSet, count);
    if (retval != PAPI_OK) handle_error(retval);

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

    PAPI_read(EventSet, values);
    
    printf("result (%s): %lld, cycles (%s): %lld\n", EVENT, values[0], CYCLES, values[1]);

    if (PAPI_stop(EventSet, values) != PAPI_OK)
        handle_error(1);

    printf(" end result (%s): %lld, cycles (%s): %lld\n", EVENT, values[0], CYCLES, values[1]);

    /* Exit successfully */
    printf("Success!\n");
    exit(0);
}

