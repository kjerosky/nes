#ifndef NES_H
#define NES_H

#include <sstream>
#include <map>

#include "olcPixelGameEngine.h"
#include "Types.h"
#include "Cpu.h"
#include "Ppu.h"
#include "Bus.h"

class Nes {

private:

    const float SECONDS_BETWEEN_FRAMES = 1.0f / 60.0f;

    Cpu* cpu;
    Ppu* ppu;
    Bus* bus;

public:

    Nes(
        nesWord mainCodeRamOffset,
        std::stringstream& mainCodeBytesStream,
        nesWord irqCodeRamOffset,
        std::stringstream& irqCodeBytesStream
    );
    ~Nes();

    olc::Sprite* getScreen();
    std::map<nesWord, std::string> disassemble(nesWord lowerAddress, nesWord upperAddress);
    void executeNextInstruction();
    void displayNextFrame();
    void toggleContinuousExecution();
    void processTimeElapsed(float secondsElapsed);
    CpuInfo getCpuInfo();
    nesByte* getCpuRam();

    void reset();
    void irq();

private:

    int cycleCounter;
    bool isContinuouslyExecuting;
    float secondsUntilNextFrameDraw;

    void clockTick();
};

#endif
