#include <iostream>
#include <vector>

// Define the struct
typedef struct ghb_entry {
    unsigned long long int addr;
    int last_addr_ptr;
} ghb_entry_t;

class GHB {
private:
    std::vector<ghb_entry_t> buffer;
    int capacity;
    int head;     // head points to the newest element
    int tail;     // tail points to the oldest element
    int size;

public:
    // Constructor
    GHB(int capacity) : capacity(capacity), head(0), tail(0), size(0) {
        buffer.resize(capacity);
    }

    // Destructor
    ~GHB() {}

    // Function to add an entry to the buffer
    void add_entry(unsigned long long int addr) {
      ghb_entry_t entry;
      if (size == 0) 
      {
        buffer[head] = entry;
      } 
      else
      {
        head = (head + 1) % capacity;
        buffer[head] = entry;
        if (head == tail) {
          tail = (tail + 1) % capacity;
        }
      }
      size = (size + 1) % capacity;

      buffer[head].addr = addr;
      // find the most recent element with the same addr
      int last_ptr = -1;
      for (int i = (head - 1) % capacity; i != (tail - 1) % capacity; i = (i - 1) % capacity) {
        if (buffer[i].addr == addr) {
            last_ptr = i;
            break;
        }
      }
      entry.last_addr_ptr = last_ptr;
    }

    bool is_full() {
        return (size == capacity);
    }
};

