// Modified code from 
// https://github.com/google/rowhammer-test/tree/master
//
// Original author: Thomas Dullien (thomasdullien@google.com)

#include <asm/unistd.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/kernel-page-flags.h>
#include <map>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <vector>

namespace {

// The fraction of physical memory that should be mapped for testing.
double fraction_of_physical_memory = 0.3;

// The number of memory reads to try.
uint64_t number_of_reads = 10000;

// Obtain the size of the physical memory of the system.
uint64_t GetPhysicalMemorySize() {
  struct sysinfo info;
  sysinfo( &info );
  return (size_t)info.totalram * (size_t)info.mem_unit;
}

uint64_t GetPageFrameNumber(int pagemap, uint8_t* virtual_address) {
  // Read the entry in the pagemap.
  uint64_t value;
  int got = pread(pagemap, &value, 8,
                  (reinterpret_cast<uintptr_t>(virtual_address) / 0x1000) * 8);
  assert(got == 8);
  uint64_t page_frame_number = value & ((1ULL << 54)-1);
  return page_frame_number;
}

void SetupMapping(uint64_t* mapping_size, void** mapping) {
  *mapping_size = 
    static_cast<uint64_t>((static_cast<double>(GetPhysicalMemorySize()) * 
          fraction_of_physical_memory));

  *mapping = mmap(NULL, *mapping_size, PROT_READ | PROT_WRITE,
      MAP_POPULATE | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  assert(*mapping != (void*)-1);

  // Initialize the mapping so that the pages are non-empty.
  for (uint64_t index = 0; index < *mapping_size; index += 0x1000) {
    uint64_t* temporary = reinterpret_cast<uint64_t*>(
        static_cast<uint8_t*>(*mapping) + index);
    temporary[0] = index;
  }
}

uint64_t HammerAddressesStandard(
    const std::pair<uint64_t, uint64_t>& first_range,
    const std::pair<uint64_t, uint64_t>& second_range,
    uint64_t number_of_reads) {
  volatile uint64_t* first_pointer =
      reinterpret_cast<uint64_t*>(first_range.first);
  volatile uint64_t* second_pointer =
      reinterpret_cast<uint64_t*>(second_range.first);
  uint64_t sum = 0;

  while (number_of_reads-- > 0) {
    sum += first_pointer[0];
    sum += second_pointer[0];
    asm volatile(
        "clflush (%0);\n\t"
        "clflush (%1);\n\t"
        : : "r" (first_pointer), "r" (second_pointer) : "memory");
  }
  return sum;
}

typedef uint64_t(HammerFunction)(
    const std::pair<uint64_t, uint64_t>& first_range,
    const std::pair<uint64_t, uint64_t>& second_range,
    uint64_t number_of_reads);

// A comprehensive test that attempts to hammer adjacent rows for a given 
// assumed row size (and assumptions of sequential physical addresses for 
// various rows.
uint64_t HammerAllReachablePages(uint64_t presumed_row_size, 
    void* memory_mapping, uint64_t memory_mapping_size, HammerFunction* hammer,
    uint64_t number_of_reads) {
  // This vector will be filled with all the pages we can get access to for a
  // given row size.
  std::vector<std::vector<uint8_t*>> pages_per_row;
  uint64_t total_bitflips = 0;

  pages_per_row.resize(memory_mapping_size / presumed_row_size);
  int pagemap = open("/proc/self/pagemap", O_RDONLY);
  assert(pagemap >= 0);

  for (uint64_t offset = 0; offset < memory_mapping_size; offset += 0x1000) {
    uint8_t* virtual_address = static_cast<uint8_t*>(memory_mapping) + offset;
    uint64_t page_frame_number = GetPageFrameNumber(pagemap, virtual_address);
    uint64_t physical_address = page_frame_number * 0x1000;
    uint64_t presumed_row_index = physical_address / presumed_row_size;
    //printf("[!] put va %lx pa %lx into row %ld\n", (uint64_t)virtual_address,
    //    physical_address, presumed_row_index);
    if (presumed_row_index > pages_per_row.size()) {
      pages_per_row.resize(presumed_row_index);
    }
    pages_per_row[presumed_row_index].push_back(virtual_address);
    //printf("[!] done\n");
  }

  // We should have some pages for most rows now.
  for (uint64_t row_index = 0; row_index + 2 < pages_per_row.size(); 
      ++row_index) {
    if ((pages_per_row[row_index].size() != 64) || 
        (pages_per_row[row_index+2].size() != 64)) {
      continue;
    } else if (pages_per_row[row_index+1].size() == 0) {
      continue;
    }
    // Iterate over all pages we have for the first row.
    for (uint8_t* first_row_page : pages_per_row[row_index]) {
      // Iterate over all pages we have for the second row.
      for (uint8_t* second_row_page : pages_per_row[row_index+2]) {
        // Set all the target pages to 0xFF.
        for (uint8_t* target_page : pages_per_row[row_index+1]) {
          memset(target_page, 0xFF, 0x1000);
        }
        // Now hammer the two pages we care about.
        std::pair<uint64_t, uint64_t> first_page_range(
            reinterpret_cast<uint64_t>(first_row_page), 
            reinterpret_cast<uint64_t>(first_row_page+0x1000));
        std::pair<uint64_t, uint64_t> second_page_range(
            reinterpret_cast<uint64_t>(second_row_page),
            reinterpret_cast<uint64_t>(second_row_page+0x1000));
        hammer(first_page_range, second_page_range, number_of_reads);
        exit(0);
      }
    }
  }
  return total_bitflips;
}

void HammerAllReachableRows(HammerFunction* hammer, uint64_t number_of_reads) {
  uint64_t mapping_size;
  void* mapping;
  SetupMapping(&mapping_size, &mapping);

  HammerAllReachablePages(1024*256, mapping, mapping_size,
                          hammer, number_of_reads);
}

}  // namespace

int main(int argc, char** argv) {

  HammerAllReachableRows(&HammerAddressesStandard, number_of_reads);
}