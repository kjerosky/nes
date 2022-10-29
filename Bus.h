#ifndef BUS_H
#define BUS_H

#include "Types.h"

class Bus {

public:

    static const int RAM_SIZE = 65536;
    nesByte ram[RAM_SIZE];

    Bus();
    ~Bus();

    nesByte cpuRead(nesWord address, bool onlyRead = false);
    void cpuWrite(nesWord address, nesByte value);
};

#endif
