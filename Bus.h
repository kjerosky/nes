#ifndef BUS_H
#define BUS_H

#include "Types.h"

class Bus {

public:

    Bus();
    ~Bus();

    byte read(word address);
    void write(word address, byte value);

private:

    static const int RAM_SIZE = 65536;

    byte ram[RAM_SIZE];
};

#endif
