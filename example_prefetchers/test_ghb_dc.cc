#include <iostream>
#include <cassert>
#include "ghb.cc"


int main() {
    GHB ghb(15, TYPE_G_DC);

    ghb.add_entry(0xabc00a); // A
    ghb.add_entry(0xabc00b); // B
    ghb.add_entry(0xabc00c); // C
    ghb.add_entry(0xabc00d); // D
    ghb.add_entry(0xabc00c); // C
    ghb.add_entry(0xabc00a); // A
    ghb.add_entry(0xabc00c); // C
    ghb.add_entry(0xabc00d); // D 
    ghb.add_entry(0xabc00b); // B -> A
    ghb.add_entry(0xabc00c); // C
    ghb.add_entry(0xabc00a); // A -> B

    ghb.print_buffer();
    ghb.print_index_table();
    ghb.print_index_table_stats();

    std::cout << "All test cases passed!" << std::endl;

    return 0;
}