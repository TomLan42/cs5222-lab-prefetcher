#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>


#define TYPE_G_AC 1
#define TYPE_G_DC 2

typedef struct index_table_entry {
    int addr_chain_len;
    int addr_head_index;
} index_table_entry_t;


typedef struct ghb_entry {
    unsigned long long int addr;
    int last_addr_ptr;
} ghb_entry_t;

class GHB {
private:
    int xy_type;
    std::unordered_map<unsigned long long int, index_table_entry_t> addr_index_table;
    std::unordered_map<long long int, index_table_entry_t> delta_index_table;
    std::vector<ghb_entry_t> buffer;
    int capacity;
    int head;     // head points to the next write position
    int size;


    std::unordered_map<unsigned long long int, unsigned long long int> addr_stats_table;
    std::unordered_map<long long int, unsigned long long int> delta_stats_table;

public:
    // Constructor
    GHB(int capacity, int type) : capacity(capacity), head(0), size(0) {
        buffer.resize(capacity);
        xy_type = type;
    }

    // Destructor
    ~GHB() {}

    void add_entry(unsigned long long int addr) {
     
      if (xy_type == TYPE_G_AC)
      {
        add_entry_ac(addr);
      }
      
      if (xy_type == TYPE_G_DC)
      {
        add_entry_dc(addr);
      }

    }

    void add_entry_ac(unsigned long long int addr) 
    {

      addr_stats_table[addr]++;


      // Remove overwritten entry
      ghb_entry_t replaced_entry =  buffer[head];
      if (replaced_entry.addr != 0) {
        auto it = addr_index_table.find(replaced_entry.addr);
        if (it != addr_index_table.end()) {
          it->second.addr_chain_len--;
          if (it->second.addr_chain_len == 0) {
            addr_index_table.erase(replaced_entry.addr);
          }
        }
      }

      // Allocate new entry
      ghb_entry_t entry;
      entry.addr = addr;

      auto it = addr_index_table.find(addr);
      if (it != addr_index_table.end()) {
        entry.last_addr_ptr = it->second.addr_head_index;

        it->second.addr_chain_len++;
        it->second.addr_head_index = head;
      } else {
        entry.last_addr_ptr = -1;

        index_table_entry_t index_table_entry;
        index_table_entry.addr_chain_len = 1;
        index_table_entry.addr_head_index = head;
        addr_index_table[addr] = index_table_entry;
      }

      buffer[head] = entry;
      head = (head + 1) % capacity;
      if (size < capacity) {
        size++;
      }

      // uncomment to check debug info
      // print_buffer();
      // print_index_table();
    }

    void add_entry_dc(unsigned long long int addr) {
      // Remove overwritten entry
      ghb_entry_t replaced_entry =  buffer[head];
      if (replaced_entry.addr != 0) {
        signed long long replaced_delta = get_delta(replaced_entry.addr, buffer[(head+1) % capacity].addr);
        auto it = delta_index_table.find(replaced_delta);
        if (it != delta_index_table.end()) {
          it->second.addr_chain_len--;
          if (it->second.addr_chain_len == 0) {
            addr_index_table.erase(replaced_delta);
          }
        }
      }

      // Allocate new entry
      ghb_entry_t entry;
      entry.addr = addr;


      signed long long new_delta = get_delta(buffer[(head-1) % capacity].addr, addr);

      delta_stats_table[new_delta]++;

      auto it = delta_index_table.find(new_delta);
      if (it != delta_index_table.end()) {
        entry.last_addr_ptr = it->second.addr_head_index;

        it->second.addr_chain_len++;
        it->second.addr_head_index = head;
      } else {
        entry.last_addr_ptr = -1;

        index_table_entry_t index_table_entry;
        index_table_entry.addr_chain_len = 1;
        index_table_entry.addr_head_index = head;
        if (buffer[(head-1) % capacity].addr != 0)
        {
          delta_index_table[new_delta] = index_table_entry;
        }
      }

      buffer[head] = entry;
      head = (head + 1) % capacity;
      if (size < capacity) {
        size++;
      }

    }

    std::vector<int> get_chained_entries_by_addr(unsigned long long int addr, int width) 
    {
      std::vector<int> result;
      int chain_head_index;
      int chain_len;
      if (xy_type == TYPE_G_AC)
      {
        auto it = addr_index_table.find(addr);
        if (it == addr_index_table.end()) 
        {
        return result;
        }
        chain_head_index = it->second.addr_head_index;
        chain_len = it->second.addr_chain_len;
      }

      
      if (xy_type == TYPE_G_DC)
      {
        signed long long delta = get_delta(addr, buffer[(head-1)%capacity].addr);
        auto it = delta_index_table.find(delta);
        if (it == delta_index_table.end()) 
        {
          return result;
        }
        chain_head_index = it->second.addr_head_index;
        chain_len = it->second.addr_chain_len;
      }

       for (int i = 0; i < (chain_len - 1) && i < width; i++) 
       {
          chain_head_index = buffer[chain_head_index].last_addr_ptr;
          result.push_back(chain_head_index);
       }
     
      return result;
    }


    ghb_entry_t get_entry_by_index(int index) 
    {
      return buffer[index];
    }

    void print_buffer() 
    {
      std::cout << "Buffer contents:" << std::endl;
      for (const auto& entry : buffer) {
        std::cout << "Address: 0x" << std::hex <<  entry.addr << ", Prev pointer: " << std::dec << entry.last_addr_ptr <<  ", Prev val:0x" <<  std::hex << buffer[entry.last_addr_ptr].addr << std::endl;
      }
      std::cout << "head: " << head << std::endl;
      std::cout << "size: " << size << std::endl;
    }

    void print_index_table() 
    {
      if (xy_type == TYPE_G_AC)
      {
        std::cout << "Address Index table contents:" << std::endl;
        for (const auto& pair : addr_index_table) 
        {
          std::cout << "Addr: " << std::hex << pair.first << ", Head: " << std::dec << pair.second.addr_head_index <<   ", Chain len: " << std::dec << pair.second.addr_chain_len <<  std::endl;
        }
        std::cout << "----------"  << std::endl;
      }
      
      if (xy_type == TYPE_G_DC)
      {
        std::cout << "Delta Index table contents:" << std::endl;
        for (const auto& pair : delta_index_table) 
        {
          std::cout << "Delta: " << std::dec << pair.first << ", Head: " << std::dec << pair.second.addr_head_index <<   ", Chain len: " << std::dec << pair.second.addr_chain_len <<  std::endl;
        }
        std::cout << "----------"  << std::endl;
      }
      
    }


    void print_index_table_stats() {
      std::map<signed long long, unsigned long long> index_table_dist;

      if (xy_type == TYPE_G_AC)
      {
        for (const auto& pair : addr_stats_table) 
        {
          index_table_dist[pair.second]++;
        }
        std::cout << "Miss Addr Distribution:" << std::endl;
        for (const auto& pair : index_table_dist) 
        {
          std::cout << "No. of occurance: " << pair.first << ", No. of addr: " << pair.second << std::endl;
        }
      }
      if (xy_type == TYPE_G_DC)
      {
        for (const auto& pair : delta_stats_table) 
        {
          index_table_dist[pair.second]++;
        }
        std::cout << "Delta Distribution:" << std::endl;
        for (const auto& pair : index_table_dist) 
        {
          std::cout << "No. of occurance: " << pair.first << ", No. of deltas: " << pair.second << std::endl;
        }
      }

    }


    bool is_full() {
        return (size == capacity);
    }

    signed long long get_delta(unsigned long long addr,  unsigned long long next_addr)
    {
      signed long long delta = static_cast<signed long long>(next_addr) - static_cast<signed long long>(addr);
      return delta;
    }

    unsigned long long apply_delta(unsigned long long addr,  signed long long delta)
    {
      if (delta < 0) {
        delta = -delta;
        return addr - static_cast<unsigned long long>(delta);
      }
    
      return addr + static_cast<unsigned long long>(delta);
    }
};



