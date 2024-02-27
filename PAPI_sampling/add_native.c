/* First template program for initializing the PAPI library,
* creating an event set and reading the measurements.
*
**********************************************************
* Resources:
*   https://bitbucket.org/icl/papi/wiki/PAPI-Overview.md
*   https://linux.die.net/man/3/papi_event_code_to_name 
*/

#include <stdlib.h>
#include <stdio.h>
#include <papi.h>

#define EVENT "CACHE-MISSES" // any event that can be measured
// the full list is contained in files all_native_events_summary.txt
// and test_for_native_events.txt in "useful system information" directory

int main()
{
    int retval, EventSet = PAPI_NULL;
    unsigned int native = 0x0;
    PAPI_event_info_t info;
    long_long values[1];

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
    
    /* Start counting */
    if (PAPI_start(EventSet) != PAPI_OK){
        printf("start error");
        handle_error(1);
    }

    /* Do some computation here */
    for(int i=1; i<10;i++){
        printf("%d\n", i);
    }

    /* Read the counters */
    if (PAPI_read(EventSet, values) != PAPI_OK){
        printf("read error");
        handle_error(1);
    }

    /* Print the result */
    printf("result (%s): %lld\n", EVENT, values[0]);

    /* Stop the counters */
    if (PAPI_stop(EventSet, values) != PAPI_OK)
        handle_error(1);
    

    /* Exit successfully */
    printf("Success!\n");
    exit(0);
}

