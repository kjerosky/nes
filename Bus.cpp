#include <algorithm>
#include "Bus.h"

using namespace std;

Bus::Bus(Ppu* ppu, Cartridge* cartridge) {
    this->ppu = ppu;
    this->cartridge = cartridge;

    fill(ram, ram + RAM_SIZE, 0);
}

Bus::~Bus() {
    // do nothing
}

nesByte Bus::cpuRead(nesWord address, bool onlyRead) {
    if (address >= 0x0000 && address <= 0x1FFF) {
        return ram[address & 0x07FF];
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        return ppu->cpuRead(address & 0x0007, onlyRead);
    } else if (address >= 0x8000 && address <= 0xFFFF) {
        return cartridge->cpuRead(address);
    }

    return 0x00;
}

void Bus::cpuWrite(nesWord address, nesByte value) {
    if (address >= 0x0000 && address <= 0x1FFF) {
        ram[address] = value;
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        return ppu->cpuWrite(address & 0x0007, value);
    } else if (address >= 0x8000 && address <= 0xFFFF) {
        return cartridge->cpuWrite(address, value);
    }
}
