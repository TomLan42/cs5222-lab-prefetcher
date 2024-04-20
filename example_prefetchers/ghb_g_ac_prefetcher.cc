#include <stdio.h>
#include "../inc/prefetcher.h"

#define INDEX_TABLE_LENGTH
#define GHB_LENGTH

// Index Table
typedef struct index_table_pair
{
    unsigned long long int addr;
    int ghb_addr_ptr;
} index_table_pair_t;

index_table_pair_t index_table[INDEX_TABLE_LENGTH];


// Global History Buffer
typedef struct ghb_entry
{
  // miss address
  unsigned long long int addr;

  // pointer to the ghb entry with the same miss address
  int last_addr_ptr;
} ghb_entry_t;

ghb_entry_t ghb[GHB_LENGTH];
int ghb_head_ptr;



void l2_prefetcher_initialize(int cpu_num)
{
  printf("No Prefetching\n");
  // you can inspect these knob values from your code to see which configuration you're runnig in
  printf("Knobs visible from prefetcher: %d %d %d\n", knob_scramble_loads, knob_small_llc, knob_low_bandwidth);
}

void l2_prefetcher_operate(int cpu_num, unsigned long long int addr, unsigned long long int ip, int cache_hit)
{
  // uncomment this line to see all the information available to make prefetch decisions
  //printf("(0x%llx 0x%llx %d %d %d) ", addr, ip, cache_hit, get_l2_read_queue_occupancy(0), get_l2_mshr_occupancy(0));
}

void l2_cache_fill(int cpu_num, unsigned long long int addr, int set, int way, int prefetch, unsigned long long int evicted_addr)
{
  // uncomment this line to see the information available to you when there is a cache fill event
  //printf("0x%llx %d %d %d 0x%llx\n", addr, set, way, prefetch, evicted_addr);
}

void l2_prefetcher_heartbeat_stats(int cpu_num)
{
  printf("Prefetcher heartbeat stats\n");
}

void l2_prefetcher_warmup_stats(int cpu_num)
{
  printf("Prefetcher warmup complete stats\n\n");
}

void l2_prefetcher_final_stats(int cpu_num)
{
  printf("Prefetcher final stats\n");
}
