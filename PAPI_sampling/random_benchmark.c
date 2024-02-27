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


#include <papi.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE (1 << 18)
#define BUFFER_SIZE_MASK (BUFFER_SIZE - 1)
#define RH_THRESHOLD (70000)
#define LF "\n"

#define FLUSH(addr) asm volatile("CLFLUSH (%[a])"::[a] "r" (addr):)



#define INSTR "INST_RETIRED:ANY"
#define MEMINSTR "MEM_INST_RETIRED:ANY"
#define L1DRA "PERF_COUNT_HW_CACHE_L1D:READ:ACCESS"

int retval = 0;
int EventSet = PAPI_NULL;
unsigned int native = 0x0;
long_long values[3];

void no_attack_random(const uint32_t nb_loops, const uint32_t *buffer)
{
    long long unsigned index;
    uintptr_t addr;
    for(int i=0;i<nb_loops;i++){
    index = rand() % 163840;
    addr = (uintptr_t) (&buffer[index]);
    asm volatile(
        "  MOV (%[addr]), %%edx \n"
        :: 
           [addr] "r" (addr)
        : "%edx", "memory"
        );
    }
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
  no_attack_random((RH_THRESHOLD+1)/2, buffer);
  FLUSH(vict.addr);
  /* printf("vict:%xh[%d|%d|%xh|%xh] = %08Xh", vict.addr, vict.bank_group, vict.bank, vict.row,
                                            vict.column, *((int*)vict.addr)); */
}

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
    retval = PAPI_event_name_to_code(INSTR, &native);
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
    
    asm volatile ("start_program:");
    for(int i=0; i< 1000; i++){
        hammer();
    }
    asm volatile ("end_program:\nnop");

    if (PAPI_stop(EventSet, values) != PAPI_OK)
        handle_error(1);

    printf("random_benchmark, %lld, %lld, %lld\n", values[0], values[1], values[2]);

    /* Exit successfully */
    exit(0);
}

