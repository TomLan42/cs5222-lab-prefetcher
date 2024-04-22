#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>



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
    std::unordered_map<unsigned long long int, index_table_entry_t> index_table;
    std::vector<ghb_entry_t> buffer;
    int capacity;
    int head;     // head points to the next write position
    int size;

public:
    // Constructor
    GHB(int capacity) : capacity(capacity), head(0), size(0) {
        buffer.resize(capacity);
    }

    // Destructor
    ~GHB() {}

    void add_entry(unsigned long long int addr) {
      // Remove overwritten entry
      ghb_entry_t replaced_entry =  buffer[head];
      if (replaced_entry.addr != 0) {
        auto it = index_table.find(replaced_entry.addr);
        if (it != index_table.end()) {
          it->second.addr_chain_len--;
          if (it->second.addr_chain_len == 0) {
            index_table.erase(replaced_entry.addr);
          }
        }
      }

      // Allocate new entry
      ghb_entry_t entry;
      entry.addr = addr;

      auto it = index_table.find(entry.addr);
      if (it != index_table.end()) {
        entry.last_addr_ptr = it->second.addr_head_index;

        it->second.addr_chain_len++;
        it->second.addr_head_index = head;
      } else {
        entry.last_addr_ptr = -1;

        index_table_entry_t index_table_entry;
        index_table_entry.addr_chain_len = 1;
        index_table_entry.addr_head_index = head;
        index_table[entry.addr] = index_table_entry;
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

    std::vector<int> get_entries_by_addr(unsigned long long int addr, int width) {
      std::vector<int> result;
     
      auto it = index_table.find(addr);
      if (it == index_table.end()) {
        return result;
      }

      int cur_index = it->second.addr_head_index;

      for (int i = 0; i < (it->second.addr_chain_len - 1) && i < width; i++) {
        cur_index = buffer[cur_index].last_addr_ptr;
        result.push_back(cur_index);
      }
      return result;
    }

    ghb_entry_t get_entry_by_index(int index) {
      return buffer[index];
    }

    void print_buffer() {
      std::cout << "Buffer contents:" << std::endl;
      for (const auto& entry : buffer) {
        std::cout << "Address: " << entry.addr << ", Last pointer: " << entry.last_addr_ptr << std::endl;
      }
      std::cout << "head: " << head << std::endl;
      std::cout << "size: " << size << std::endl;
    }

    void print_index_table() {
      std::cout << "Index table contents:" << std::endl;
      for (const auto& pair : index_table) {
        std::cout << "Addr: " << pair.first << ", Head: " << pair.second.addr_head_index <<  ", Chain len: " << pair.second.addr_chain_len <<  std::endl;
      }
      std::cout << "----------"  << std::endl;
    }


    void print_index_table_stats() {
      std::map<int, int> index_table_dist;

      for (const auto& pair : index_table) {
        index_table_dist[pair.second.addr_chain_len]++;
      }

      std::cout << "Index Table Dist contents:" << std::endl;
      for (const auto& pair : index_table_dist) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
      }
    }


    bool is_full() {
        return (size == capacity);
    }
};

