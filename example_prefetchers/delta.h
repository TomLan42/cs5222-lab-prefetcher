
#ifndef DELTA_H
#define DELTA_H

signed long long get_delta(unsigned long long addr,  unsigned long long next_addr);

unsigned long long apply_delta(unsigned long long addr,  signed long long delta);

#endif // DELTA_H

