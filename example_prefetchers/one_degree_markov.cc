#include <stdio.h>
#include "../inc/prefetcher.h"
#include "ghb.cc"
#include <set>

#define ROW_WIDTH 1

typedef struct markov_row {
    int head;
    std::vector<unsigned long long int> row;
} markov_row_t;

std::unordered_map<unsigned long long int, markov_row_t> markov_table;

unsigned long long int prev_addr;

std::unordered_map<std::string, int> one_degree_hit_count;

std::unordered_map<int, int> pf_success_count;


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
    auto it = markov_table.find(prev_addr);
    if (it != markov_table.end())
    {
      for (int next_addr : it->second.row) {
        if (next_addr != 0)
        {
          l2_prefetch_line(0, addr, next_addr, FILL_L2);
        }
      }
      it->second.row[it->second.head] = addr;
      it->second.head = (it->second.head + 1) % ROW_WIDTH;
    }
    else
    {
      markov_table[prev_addr].row.resize(ROW_WIDTH);
      markov_table[prev_addr].row[0] = addr;
      markov_table[prev_addr].head = 1 % ROW_WIDTH;
    }
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
  // ghb.print_index_table();
  // ghb.print_index_table_stats();

  printf("Prefetcher final stats\n");
}
