// Modified code from 
// https://github.com/google/rowhammer-test/tree/master

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>

const size_t mem_size = 1 << 30;
const int toggles = 10000;

char *g_mem;


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

struct timeval start_time_;

double get_diff() {
    struct timeval end_time;
    int rc = gettimeofday(&end_time, NULL);
    assert(rc == 0);
    return (end_time.tv_sec - start_time_.tv_sec
            + (double) (end_time.tv_usec - start_time_.tv_usec) / 1e6);
  }

  void main_prog() {
  g_mem = (char *) mmap(NULL, mem_size, PROT_READ | PROT_WRITE,
                        MAP_ANON | MAP_PRIVATE, -1, 0);
  assert(g_mem != MAP_FAILED);

  memset(g_mem, 0xff, mem_size);


  
    /* Initialize the timer */
    gettimeofday(&start_time_, NULL);


  toggle(1000, 8); //iterations, addr_count

  double diff = get_diff();

    /*Count the time taken*/

    printf("rh8, %lf  sec\n", diff/1000);

}

int main()
{

    /* INSERT THE PROGRAM HERE */ 
    
    main_prog();


    /* Exit successfully */
    exit(0);
}

