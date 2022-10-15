#ifndef BUS_H
#define BUS_H

#include "Types.h"

class Bus {

public:

    Bus();
    ~Bus();

    nesByte read(nesWord address);
    void write(nesWord address, nesByte value);

private:

    static const int RAM_SIZE = 65536;

    nesByte ram[RAM_SIZE];
};

#endif
