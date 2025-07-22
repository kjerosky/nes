#include <algorithm>
#include "Bus.h"

using namespace std;

Bus::Bus(Ppu* ppu, Apu* apu, Cartridge* cartridge) {
    this->ppu = ppu;
    this->apu = apu;
    this->cartridge = cartridge;

    fill(ram, ram + RAM_SIZE, 0);
}

Bus::~Bus() {
    // do nothing
}

Uint8 Bus::cpuRead(Uint16 address, bool onlyRead) {
    if (address >= 0x0000 && address <= 0x1FFF) {
        return ram[address & 0x07FF];
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        return ppu->cpuRead(address & 0x0007, onlyRead);
    } else if (address >= 0x4016 && address <= 0x4017) {
        int controllerId = address & 0x0001;
        Uint8 currentControllerBit = latchedControllerStates[controllerId] & 0x01;
        latchedControllerStates[controllerId] >>= 1;
        return currentControllerBit;
    } else if (address >= 0x8000 && address <= 0xFFFF) {
        return cartridge->cpuRead(address);
    }

    return 0x00;
}

void Bus::cpuWrite(Uint16 address, Uint8 value) {
    if (address >= 0x0000 && address <= 0x1FFF) {
        ram[address] = value;
    } else if (address >= 0x2000 && address <= 0x3FFF) {
        ppu->cpuWrite(address & 0x0007, value);
    } else if ((address >= 0x4000 && address <= 0x4013) || address == 0x4015 || address == 0x4017) {
        apu->cpuWrite(address, value);
    } else if (address == 0x4014) {
        dmaPage = value;
        dmaAddress = 0x00;
        dmaReadyToWriteData = false;
        dmaInProgress = true;
    } else if (address >= 0x4016 && address <= 0x4017) {
        // NOTE: There are more accurate ways to do this, but it works for now! :)
        int controllerId = address & 0x0001;
        latchedControllerStates[controllerId] = controllerStates[controllerId];
    } else if (address >= 0x8000 && address <= 0xFFFF) {
        cartridge->cpuWrite(address, value);
    }
}

void Bus::updateControllerStates(Uint8 controller1State, Uint8 controller2State) {
    controllerStates[0] = controller1State;
    controllerStates[1] = controller2State;
}

bool Bus::checkDmaProgress() {
    if (!dmaInProgress) {
        return false;
    }

    if (dmaReadyToWriteData) {
        ppu->writeToOam(dmaAddress++, dmaData);

        if (dmaAddress == 0x00) {
            dmaInProgress = false;
        }
    } else {
        dmaData = cpuRead((dmaPage << 8) | dmaAddress);
    }

    dmaReadyToWriteData = !dmaReadyToWriteData;
    return true;
}
