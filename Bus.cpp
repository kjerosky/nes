#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Bus.h"

using namespace std;

Bus::Bus() {
    fill(ram, ram + RAM_SIZE, 0);
}

Bus::~Bus() {
    // do nothing
}

byte Bus::read(word address) {
    if (address >= 0x0000 && address <= 0xFFFF) {
        return ram[address];
    }

    cerr << "[ERROR] Address $" << uppercase << setfill('0') << setw(2) << right << hex << (int)address
        << " is not setup to be handled in the bus!" << endl;
    return 0;
}

void Bus::write(word address, byte value) {
    if (address >= 0x0000 && address <= 0xFFFF) {
        ram[address] = value;
    }
}
