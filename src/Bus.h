#ifndef BUS_H
#define BUS_H

#include <SDL3/SDL.h>

#include "Ppu.h"
#include "Apu.h"
#include "Cartridge.h"
#include "SaveRam.h"

class Bus {

public:

    static const int RAM_SIZE = 2048;
    Uint8 ram[RAM_SIZE];

    Bus(Ppu* ppu, Apu* apu, Cartridge* cartridge);
    ~Bus();

    Uint8 cpuRead(Uint16 address, bool onlyRead = false);
    void cpuWrite(Uint16 address, Uint8 value);

    void updateControllerStates(Uint8 controller1State, Uint8 controller2State);

    bool checkDmaProgress();

private:

    Ppu* ppu;
    Apu* apu;
    Cartridge* cartridge;
    SaveRam save_ram;

    Uint8 controllerStates[2];
    Uint8 latchedControllerStates[2];

    Uint8 dmaPage;
    Uint8 dmaAddress;
    Uint8 dmaData;
    bool dmaInProgress;
    bool dmaReadyToWriteData;
};

#endif
