#ifndef BUS_H
#define BUS_H

#include "Types.h"

class Bus {

public:

    static const int RAM_SIZE = 65536;
    nesByte ram[RAM_SIZE];

    Bus();
    ~Bus();

    nesByte read(nesWord address, bool onlyRead = false);
    void write(nesWord address, nesByte value);
};

#endif
