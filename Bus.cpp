#include <algorithm>
#include "Bus.h"

using namespace std;

Bus::Bus() {
    fill(ram, ram + RAM_SIZE, 0);
}

Bus::~Bus() {
    // do nothing
}

nesByte Bus::cpuRead(nesWord address, bool onlyRead) {
    if (address >= 0x0000 && address <= 0xFFFF) {
        return ram[address];
    }

    return 0;
}

void Bus::cpuWrite(nesWord address, nesByte value) {
    if (address >= 0x0000 && address <= 0xFFFF) {
        ram[address] = value;
    }
}
