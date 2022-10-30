#ifndef PPU_H
#define PPU_H

#include "olcPixelGameEngine.h"

class Ppu {

public:

    Ppu();
    ~Ppu();

    olc::Sprite* getScreen();
    void clockTick();
    bool isFrameComplete();
    void acknowledgeFrameWasCompleted();

private:

    olc::Pixel palette[0x40];
    olc::Sprite screen;

    int scanline;
    int cycle;
    bool frameIsComplete;

    void initializePalette();
};

#endif
