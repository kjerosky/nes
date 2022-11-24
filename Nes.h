#ifndef NES_H
#define NES_H

#include <sstream>
#include <map>

#include "olcPixelGameEngine.h"
#include "Types.h"
#include "Cpu.h"
#include "Ppu.h"
#include "Bus.h"
#include "Cartridge.h"

class Nes {

private:

    const float SECONDS_BETWEEN_FRAMES = 1.0f / 60.0f;

    Cpu* cpu;
    Ppu* ppu;
    Bus* bus;
    Cartridge* cartridge;

public:

    Nes(Cartridge* cartridge);
    ~Nes();

    olc::Sprite* getScreen();
    std::map<nesWord, std::string> disassemble(nesWord lowerAddress, nesWord upperAddress);
    void executeNextInstruction();
    void displayNextFrame();
    void toggleContinuousExecution();
    void processTimeElapsed(float secondsElapsed);
    CpuInfo getCpuInfo();
    nesByte* getCpuRam();
    olc::Sprite* getPatternTable(int patternTableIndex, int paletteIndex);
    olc::Pixel* getActivePalettesColors();
    nesByte* getNameTable(int nameTableIndex);

    void reset();
    void irq();
    void nmi();

    void updateControllerStates(nesByte controller1State, nesByte controller2State);

    nesByte* getOamBytes();

private:

    int cycleCounter;
    bool isContinuouslyExecuting;
    float secondsUntilNextFrameDraw;

    void clockTick();
};

#endif
