#include <stdio.h>
#include "../inc/prefetcher.h"
#include "ghb.cc"
#include <set>
#include <climits>

#define ROW_WIDTH 8
#define MAX_ROW_NUM 1024

typedef struct markov_row {
    int head;
    std::vector<unsigned long long int> data;
    unsigned long long lru_cycle;
} markov_row_t;

std::unordered_map<unsigned long long int, markov_row_t> markov_table;

unsigned long long int prev_addr;

std::unordered_map<std::string, int> one_degree_pattern_count;
std::unordered_map<std::string, int> one_degree_hit_count;

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

  // debug
  std::string str1 = std::to_string(prev_addr);
  std::string str2 = std::to_string(addr);
  std::string concatenated = str1 + ", " + str2;
  one_degree_pattern_count[concatenated]++;

  auto it = markov_table.find(addr);
  if (it != markov_table.end())
  {
    // fetch the most recent records
    for(size_t i = 0; i < it->second.data.size(); i++){
      if (it->second.data[i] != 0)
      {
        unsigned long long prefetch_addr = it->second.data[i];
        l2_prefetch_line(0, addr, prefetch_addr, FILL_L2);

        std::string str1 = std::to_string(addr);
        std::string str2 = std::to_string(prefetch_addr);
        std::string concatenated = str1 + ", " + str2;
        auto it = one_degree_pattern_count.find(concatenated);
        if (it != one_degree_pattern_count.end())
        {
          one_degree_hit_count[concatenated]++;
        }
      }
    }
    it->second.lru_cycle = get_current_cycle(0);
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

  // clear lru row
  if (markov_table.size() > MAX_ROW_NUM)
  {
    unsigned long long int oldest_lru_cycle = ULLONG_MAX;
    unsigned long long int oldest_lru_key = 0;
    for (const auto& pair : markov_table) {
      if (pair.second.lru_cycle < oldest_lru_cycle)
      {
        oldest_lru_cycle = pair.second.lru_cycle;
        oldest_lru_key = pair.first;
      }
    }
    markov_table.erase(oldest_lru_key);
  }

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
