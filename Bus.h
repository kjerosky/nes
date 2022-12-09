#ifndef BUS_H
#define BUS_H

#include "Types.h"
#include "Ppu.h"
#include "Apu.h"
#include "Cartridge.h"

class Bus {

public:

    static const int RAM_SIZE = 2048;
    nesByte ram[RAM_SIZE];

    Bus(Ppu* ppu, Apu* apu, Cartridge* cartridge);
    ~Bus();

    nesByte cpuRead(nesWord address, bool onlyRead = false);
    void cpuWrite(nesWord address, nesByte value);

    void updateControllerStates(nesByte controller1State, nesByte controller2State);

    bool checkDmaProgress();

private:

    Ppu* ppu;
    Apu* apu;
    Cartridge* cartridge;

    nesByte controllerStates[2];
    nesByte latchedControllerStates[2];

    nesByte dmaPage;
    nesByte dmaAddress;
    nesByte dmaData;
    bool dmaInProgress;
    bool dmaReadyToWriteData;
};

#endif
