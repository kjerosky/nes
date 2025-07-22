#ifndef NES_H
#define NES_H

#include <sstream>
#include <map>
#include <SDL3/SDL.h>

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

    std::map<Uint16, std::string> disassemble(Uint16 lowerAddress, Uint16 upperAddress);
    void executeNextInstruction();
    void displayNextFrame();
    void toggleContinuousExecution();
    void processTimeElapsed(float secondsElapsed);
    CpuInfo getCpuInfo();
    Uint8* getCpuRam();
    Uint8* getNameTable(int nameTableIndex);

    void setAudioSampleRate(int sampleRate);
    float getAudioSample();
    bool clockForAudioSample();

    void reset();
    void irq();
    void nmi();

    void updateControllerStates(Uint8 controller1State, Uint8 controller2State);

    Uint8* getOamBytes();

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
