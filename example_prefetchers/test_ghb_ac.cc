#include <iostream>
#include <cassert>
#include "ghb.cc"


int main() {
    GHB ghb(10, TYPE_G_AC);

    ghb.add_entry(0xabcdef);
    ghb.add_entry(0xdeadbeef);
    ghb.add_entry(0xabcdef);
    ghb.add_entry(0xdeadbeef);
    ghb.add_entry(0xabcdef); 
    ghb.add_entry(0xdeadbeef);
    ghb.add_entry(0xabcdef);
    ghb.add_entry(0xdeadbeef);
    ghb.add_entry(0xabcdef);
    ghb.add_entry(0xdeadbeef);
    ghb.add_entry(0xabcdef);

    ghb.print_buffer();


    ghb_entry_t entry = ghb.get_entry_by_index(0);
    assert(entry.addr == 0xabcdef);
    assert(entry.last_addr_ptr == 8);

    std::vector<int> width_indices = ghb.get_chained_entries_by_addr(0xabcdef, 2);
    for (const auto& index : width_indices) {
      std::cout << index <<  std::endl;
    }
    std::cout << "All test cases passed!" << std::endl;

    return 0;
}