#include <stdio.h>
#include "../inc/prefetcher.h"
#include "ghb.cc"
#include <set>

#define ROW_WIDTH 2

typedef struct markov_row {
    int head;
    std::vector<unsigned long long int> data;
} markov_row_t;

std::unordered_map<unsigned long long int, markov_row_t> markov_table;

unsigned long long int prev_addr;

int hit_count;


void l2_prefetcher_initialize(int cpu_num)
{
  // you can inspect these knob values from your code to see which configuration you're runnig in
  printf("Knobs visible from prefetcher: %d %d %d\n", knob_scramble_loads, knob_small_llc, knob_low_bandwidth);
}

void l2_prefetcher_operate(int cpu_num, unsigned long long int addr, unsigned long long int ip, int cache_hit)
{
  // uncomment this line to see all the information available to make prefetch decisions
  // printf("(0x%llx 0x%llx %d %d %d) \n", addr, ip, cache_hit, get_l2_read_queue_occupancy(0), get_l2_mshr_occupancy(0));

  if (cache_hit) {
    return;
  }

  auto it = markov_table.find(addr);
  if (it != markov_table.end())
  {
    for(size_t i = 0; i < it->second.data.size(); i++){
      l2_prefetch_line(0, addr, it->second.data[i], FILL_L2);
    }

  }

  auto it2 = markov_table.find(prev_addr);
  if (it2 == markov_table.end())
  {
    markov_row_t new_row;
    new_row.head = 0;
    new_row.data.resize(ROW_WIDTH);
    markov_table[prev_addr] = new_row;
  }

  markov_table[prev_addr].data[markov_table[prev_addr].head] = addr;
  markov_table[prev_addr].head = (markov_table[prev_addr].head + 1) % ROW_WIDTH;

  prev_addr = addr;

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
  // ghb.print_index_table();
  // ghb.print_index_table_stats();

  printf("Prefetcher final stats\n");
  printf("Hit count: %d \n", hit_count);

  // Print the contents of the markov_table
  for (const auto& pair : markov_table) {
      std::cout << "Key: " << pair.first << ", Head: " << pair.second.head << ", Data: ";
      for (const auto& data : pair.second.data) {
          std::cout << data << " ";
      }
      std::cout << std::endl;
  }

}
