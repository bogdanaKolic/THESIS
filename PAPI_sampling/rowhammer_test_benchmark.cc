/* A program that serves as a benchmark for computing the
* total number of event occurences without periodic readings.
*
**********************************************************
* Modified code from 
* https://github.com/google/rowhammer-test/tree/master */


#include <papi.h>
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

const size_t mem_size = 1 << 30;
const int toggles = 100000;

char *g_mem;


#define INSTR "INST_RETIRED:ANY"
#define INSTR_LENOVO "INST_RETIRED:ANY_P"
#define MEMINSTR "MEM_INST_RETIRED:ANY"
#define L1DRA "PERF_COUNT_HW_CACHE_L1D:READ:ACCESS"

void handle_error (int retval)
{
     printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
     exit(1);
}

int retval = 0;
int EventSet = PAPI_NULL;
int native = 0;
long_long values[3];

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


  /* Start counting */
  if (PAPI_start(EventSet) != PAPI_OK){
      printf("start error");
      handle_error(1);
  }

  toggle(5, 4); //iterations, addr_count

  if (PAPI_stop(EventSet, values) != PAPI_OK)
      handle_error(1);

  printf("rh_benchmark, %lld, %lld, %lld\n", values[0], values[1], values[2]);

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

    /* INSERT THE PROGRAM HERE */ 
    
    main_prog();


    /* Exit successfully */
    exit(0);
}

