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
    } else if (address >= 0x4016 && address <= 0x4017) {
        int controllerId = address & 0x0001;
        nesByte currentControllerBit = latchedControllerStates[controllerId] & 0x01;
        latchedControllerStates[controllerId] >>= 1;
        return currentControllerBit;
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
    } else if (address >= 0x4016 && address <= 0x4017) {
        // NOTE: There are more accurate ways to do this, but it works for now! :)
        int controllerId = address & 0x0001;
        latchedControllerStates[controllerId] = controllerStates[controllerId];
    } else if (address >= 0x8000 && address <= 0xFFFF) {
        return cartridge->cpuWrite(address, value);
    }
}

void Bus::updateControllerStates(nesByte controller1State, nesByte controller2State) {
    controllerStates[0] = controller1State;
    controllerStates[1] = controller2State;
}
