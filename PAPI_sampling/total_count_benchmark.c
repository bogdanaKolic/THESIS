/* A program that serves as a benchmark for computing the
* total number of event occurences without periodic readings.
*
**********************************************************
* Resources:
*   https://stackoverflow.com/a/65571169 
*   https://linux.die.net/man/2/setitimer 
*   https://bitbucket.org/icl/papi/wiki/PAPI-Overview.md
*   https://linux.die.net/man/3/papi_event_code_to_name 
*   https://icl.utk.edu/projects/papi/files/documentation/PAPI_USER_GUIDE_23.htm#WHAT_IS_MULTIPLEXING 
*   https://www.mankier.com/3/PAPI_set_multiplex */


#include <stdlib.h>
#include <stdio.h>
#include <papi.h>

#define INSTR "INST_RETIRED:ANY"
#define INSTR_LENOVO "INST_RETIRED:ANY_P"
#define MEMINSTR "MEM_INST_RETIRED:ANY"
#define L1DRA "PERF_COUNT_HW_CACHE_L1D:READ:ACCESS"

int retval = 0;
int EventSet = PAPI_NULL;
unsigned int native = 0x0;
long_long values[3];


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

    /* Start counting */
    if (PAPI_start(EventSet) != PAPI_OK){
        printf("start error");
        handle_error(1);
    }

    /* INSERT THE PROGRAM HERE */ 
    


    if (PAPI_stop(EventSet, values) != PAPI_OK)
        handle_error(1);

    printf("sampled_code_benchmark, %lld, %lld, %lld\n", values[0], values[1], values[2]);

    /* Exit successfully */
    exit(0);
}

