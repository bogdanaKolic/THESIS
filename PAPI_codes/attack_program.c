#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <papi.h>

#define BUFFER_SIZE (1 << 18)
#define BUFFER_SIZE_MASK (BUFFER_SIZE - 1)
#define RH_THRESHOLD (10000)
#define LF "\n"

#define IXCREF "ix86arch::LLC_REFERENCES"
#define IXCMISS "ix86arch::LLC_MISSES"
#define L2H "L2_RQSTS:DEMAND_DATA_RD_HIT"
#define L3H "MEM_LOAD_RETIRED:L3_HIT"
#define L1H "MEM_LOAD_RETIRED:L1_HIT"
#define L1DM "PERF_COUNT_HW_CACHE_L1D:MISS"
#define L1DA "PERF_COUNT_HW_CACHE_L1D:ACCESS"
#define L1IM "PERF_COUNT_HW_CACHE_L1I:MISS"
#define L1IR "PERF_COUNT_HW_CACHE_L1I:READ"
#define STALL "UOPS_RETIRED:STALL_CYCLES"
#define MEMSTALL "CYCLE_ACTIVITY:STALLS_MEM_ANY"
#define FBHIT "MEM_LOAD_RETIRED:FB_HIT"

#define ATTACK 1
#define NO_FLUSH 2
#define RANDOM_ACCESS 3

#define FLUSH(addr) asm volatile("CLFLUSH (%[a])"::[a] "r" (addr):)


int retval, EventSet = PAPI_NULL;
unsigned int native = 0x0;
long_long values[4];
long long int run_count = 0;

void attack(const uint32_t nb_loops, const uintptr_t addr1, const uintptr_t addr2)
{
    /* Start counting */
    if (PAPI_start(EventSet) != PAPI_OK){
        printf("start error");
        handle_error(1);
    }

    asm volatile(
        "MOV %[nb_loops], %%ecx  \n"
        "JMP hammer_loop_asm     \n"
        "hammer_loop_asm:        \n"
        "  MOV (%[addr1]), %%edx \n"
        "  MOV (%[addr2]), %%edx \n"
        "  CLFLUSH (%[addr1])    \n"
        "  CLFLUSH (%[addr2])    \n"
	      "  LOOP hammer_loop_asm  \n"
        :: [nb_loops] "r" (nb_loops),
	   [addr1] "r" (addr1),
           [addr2] "r" (addr2)
        : "%ecx", "%edx", "memory"
        );

    /* Read the counters */
    if (PAPI_stop(EventSet, values) != PAPI_OK){
        printf("read error");
        handle_error(1);
    }

    /* Print the result */
    print_result(ATTACK);
}

void no_attack_no_flush(const uint32_t nb_loops, const uintptr_t addr1, const uintptr_t addr2)
{
    /* Start counting */
    if (PAPI_start(EventSet) != PAPI_OK){
        printf("start error");
        handle_error(1);
    }

    asm volatile(
        "MOV %[nb_loops], %%ecx  \n"
        "JMP no_hammer_loop_asm     \n"
        "no_hammer_loop_asm:        \n"
        "  MOV (%[addr1]), %%edx \n"
        "  MOV (%[addr2]), %%edx \n"
	      "  LOOP no_hammer_loop_asm  \n"
        :: [nb_loops] "r" (nb_loops),
	   [addr1] "r" (addr1),
           [addr2] "r" (addr2)
        : "%ecx", "%edx", "memory"
        );

    /* Read the counters */
    if (PAPI_stop(EventSet, values) != PAPI_OK){
        printf("read error");
        handle_error(1);
    }

    /* Print the result */
    print_result(NO_FLUSH);
}

void no_attack_random(const uint32_t nb_loops, const uint32_t *buffer)
{
    long long unsigned index;
    uintptr_t addr;
    /* Start counting */
    if (PAPI_start(EventSet) != PAPI_OK){
        printf("start error");
        handle_error(1);
    }

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

    /* Read the counters */
    if (PAPI_stop(EventSet, values) != PAPI_OK){
        printf("read error");
        handle_error(1);
    }

    /* Print the result */
    print_result(RANDOM_ACCESS);
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
  attack((RH_THRESHOLD+1)/2, agg1.addr, agg2.addr);
  FLUSH(vict.addr);
  no_attack_no_flush((RH_THRESHOLD+1)/2, agg1.addr, agg2.addr);
  FLUSH(vict.addr);
  no_attack_random((RH_THRESHOLD+1)/2, buffer);
  FLUSH(vict.addr);
  /* printf("vict:%xh[%d|%d|%xh|%xh] = %08Xh", vict.addr, vict.bank_group, vict.bank, vict.row,
                                            vict.column, *((int*)vict.addr)); */
}


void print_result(int loop_type) {

    // printf("%d ", loop_type);
    switch(loop_type){
        case 1:
        printf("attack: "); break;
        case 2:
        printf("no flushing: "); break;
        case 3:
        printf("random accesses: "); break;
    }
    printf("%lld %lld %d\n", run_count++, values[0], loop_type);
}

int main() {

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
    retval = PAPI_event_name_to_code(STALL, &native);
    if (retval != PAPI_OK){
        printf("event error");
        handle_error(retval);
    }


    /* Add it to the eventset */
    retval = PAPI_add_event(EventSet, native);
    if (retval != PAPI_OK) handle_error(retval);

  asm volatile ("start_program:");
  for(int i=0; i< 1000; i++){
    hammer();
    printf("\n");
  }
  asm volatile ("end_program:\nnop");
  return 0;
}