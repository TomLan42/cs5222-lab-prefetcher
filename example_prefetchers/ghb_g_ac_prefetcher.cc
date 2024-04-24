#include <stdio.h>
#include "../inc/prefetcher.h"
#include "ghb.cc"
#include <set>

#define GHB_LENGTH 1024
#define PREFETCH_WIDTH 2
#define PREFETCH_DEPTH 2

GHB ghb(GHB_LENGTH);

int issued_count;

void l2_prefetcher_initialize(int cpu_num)
{
  // you can inspect these knob values from your code to see which configuration you're runnig in
  printf("Knobs visible from prefetcher: %d %d %d\n", knob_scramble_loads, knob_small_llc, knob_low_bandwidth);
}

void l2_prefetcher_operate(int cpu_num, unsigned long long int addr, unsigned long long int ip, int cache_hit)
{
  // uncomment this line to see all the information available to make prefetch decisions
  // printf("(0x%llx 0x%llx %d %d %d) \n", addr, ip, cache_hit, get_l2_read_queue_occupancy(0), get_l2_mshr_occupancy(0));

 
  if (!cache_hit) {

    ghb.add_entry(addr);

    // std::set<unsigned long long int> prefetch_addr_set;

    // // get width
    // std::vector<int> width_indices = ghb.get_entries_by_addr(addr, PREFETCH_WIDTH);

    // // get depth
    // for (size_t i = 0; i < width_indices.size(); i++) {
    //   int index = width_indices[i];
    //   for (int j = 0; j < PREFETCH_DEPTH; j++) {
    //     ghb_entry_t entry = ghb.get_entry_by_index((index + 1 + j) % GHB_LENGTH);
    //     if (entry.addr != 0) {
    //       prefetch_addr_set.insert(entry.addr);
    //     }
    //   }
    // }


    // for (int prefetch_addr : prefetch_addr_set) {
    //   l2_prefetch_line(0, addr, prefetch_addr, FILL_L2);
    //   issued_count++;
    // }
  }

  return;
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

  printf("prefetch count: %d\n", issued_count);

  ghb.print_index_table_stats();
}
