#include <stdio.h>
#include "../inc/prefetcher.h"
#include "ghb.cc"
#include <set>

#define GHB_LENGTH 10000
#define PREFETCH_WIDTH 4
#define PREFETCH_DEPTH 4

GHB ghb(GHB_LENGTH, TYPE_G_AC);


unsigned long long prev_addr;

std::unordered_map<std::string, int> one_degree_pattern_count;
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

    // debug
    std::string str1 = std::to_string(prev_addr);
    std::string str2 = std::to_string(addr);
    std::string concatenated = str1 + ", " + str2;
    one_degree_pattern_count[concatenated]++;
    prev_addr = addr;

    ghb.add_entry(addr);

    // get width
    std::vector<int> width_indices = ghb.get_chained_entries_by_addr(addr, PREFETCH_WIDTH);

    // get depth
    for (size_t i = 0; i < width_indices.size(); i++) {
      int index = width_indices[i];
      for (int j = 0; j < PREFETCH_DEPTH; j++) {
        ghb_entry_t entry = ghb.get_entry_by_index((index + 1 + j) % GHB_LENGTH);
        if (entry.addr != 0) {
          std::string str1 = std::to_string(addr);
          std::string str2 = std::to_string(entry.addr);
          std::string concatenated = str1 + ", " + str2;
          auto it = one_degree_pattern_count.find(concatenated);
          if (it != one_degree_pattern_count.end())
          {
            one_degree_hit_count[concatenated]++;
          }
          l2_prefetch_line(0, addr, entry.addr, FILL_L2);      
        }
      }
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

  std::map<signed long long, unsigned long long> pattern_dist;
  for (const auto& pair : one_degree_pattern_count) {
    pattern_dist[pair.second]++;
  }

  std::cout << "1st Degree pattern Distribution:" << std::endl;
  for (const auto& pair : pattern_dist) 
  {
    std::cout << "No. of occurance: " << pair.first << ", No. of patterns: " << pair.second << std::endl;
  }
  std::cout << "------------------------" << std::endl;


  std::map<signed long long, unsigned long long> pattern_hit_dist;
  for (const auto& pair : one_degree_hit_count) {
    pattern_hit_dist[pair.second]++;
  }

  std::cout << "1st Degree pattern hit Distribution:" << std::endl;
  for (const auto& pair : pattern_hit_dist) 
  {
    std::cout << "No. of hits: " << pair.first << ", No. of patterns: " << pair.second << std::endl;
  }
  std::cout << "------------------------" << std::endl;


}
