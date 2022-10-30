#ifndef PPU_H
#define PPU_H

#include "olcPixelGameEngine.h"
#include "Cartridge.h"
#include "Types.h"

class Ppu {

public:

    Ppu(Cartridge* cartridge);
    ~Ppu();

    olc::Sprite* getScreen();
    void clockTick();
    bool isFrameComplete();
    void acknowledgeFrameWasCompleted();

    nesWord cpuRead(nesWord address, bool onlyRead = false);
    void cpuWrite(nesWord address, nesByte data);

private:

    Cartridge* cartridge;

    olc::Pixel palette[0x40];
    olc::Sprite screen;

    int scanline;
    int cycle;
    bool frameIsComplete;

    void initializePalette();
};

#endif
