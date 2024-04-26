#include "delta.h" 

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
