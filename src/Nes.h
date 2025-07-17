#ifndef NES_H
#define NES_H

#include <sstream>
#include <map>

#include "Types.h"
#include "Cpu.h"
#include "Ppu.h"
#include "Bus.h"
#include "Cartridge.h"
#include "Apu.h"

class Nes {

private:

    const float SECONDS_BETWEEN_FRAMES = 1.0f / 60.0f;

    Cpu* cpu;
    Ppu* ppu;
    Bus* bus;
    Cartridge* cartridge;
    Apu* apu;

public:

    Nes(Cartridge* cartridge, SDL_Texture* screen_texture, const SDL_PixelFormatDetails* pixel_format_details);
    ~Nes();

    std::map<nesWord, std::string> disassemble(nesWord lowerAddress, nesWord upperAddress);
    void executeNextInstruction();
    void displayNextFrame();
    void toggleContinuousExecution();
    void processTimeElapsed(float secondsElapsed);
    CpuInfo getCpuInfo();
    nesByte* getCpuRam();
    nesByte* getNameTable(int nameTableIndex);

    void setAudioSampleRate(int sampleRate);
    float getAudioSample();
    bool clockForAudioSample();

    void reset();
    void irq();
    void nmi();

    void updateControllerStates(nesByte controller1State, nesByte controller2State);

    nesByte* getOamBytes();

private:

    int cycleCounter;
    bool isContinuouslyExecuting;
    float secondsUntilNextFrameDraw;

    double audioSampleTime;
    double nesClockCycleTime;
    double audioTime;
    float audioSample;

    void clockTick();
};

#endif
