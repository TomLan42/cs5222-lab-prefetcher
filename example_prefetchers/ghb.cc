#include <iostream>
#include <vector>
#include <unordered_map>


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
      // Init new entry
      ghb_entry_t entry;
      entry.addr = addr;
      // find the most recent element with the same addr
      int last_ptr = -1;
      for (int count = 0; count < size - 1; count++) {
        int index = (head - 1 - count) % capacity;
        if (buffer[index].addr == addr) {
            last_ptr = index;
            break;
        }
      }
      entry.last_addr_ptr = last_ptr;


      // Allocate new entry
      ghb_entry_t replaced_entry =  buffer[head];
      buffer[head] = entry;
      head = (head + 1) % capacity;
      if (size < capacity) {
        size++;
      }

      // Update index table
      if (replaced_entry.addr != 0) {
        auto it = index_table.find(replaced_entry.addr);
        if (it != index_table.end()) {
          it->second.addr_chain_len--;
          if (it->second.addr_chain_len == 0) {
            index_table.erase(replaced_entry.addr);
          }
        }
      }

      auto it = index_table.find(entry.addr);
      if (it != index_table.end()) {
        it->second.addr_chain_len++;
      } else {
        index_table_entry_t index_table_entry;
        index_table_entry.addr_chain_len = 1;
        index_table_entry.addr_head_index = head - 1;
        index_table[entry.addr] = index_table_entry;
      }
    }

    std::vector<int> get_entries_by_addr(unsigned long long int addr, int width) {
      std::vector<int> result;
     
      auto it = index_table.find(addr);
      if (it == index_table.end()) {
        return result;
      }

      int cur_index = it->second.addr_head_index;

      for (int i = 0; i < it->second.addr_chain_len && i < width; i++) {
        result.push_back(cur_index);
        cur_index = buffer[i].last_addr_ptr;
      }
      return result;
    }

    ghb_entry_t get_entry_by_index(int index) {
      return buffer[index];
    }

    bool is_full() {
        return (size == capacity);
    }
};

