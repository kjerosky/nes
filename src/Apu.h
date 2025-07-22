#ifndef APU_H
#define APU_H

#include <vector>

#include "PulseChannel.h"
#include "TriangleChannel.h"
#include "NoiseChannel.h"

class Apu {

public:

    Apu();
    ~Apu();

    void reset();
    void clockTick();
    float getAudioSampleOutput();

    Uint8 cpuRead(Uint16 address, bool onlyRead = false);
    void cpuWrite(Uint16 address, Uint8 data);

private:

    std::vector<Uint8> lengthsTable;

    int cycleCount;

    PulseChannel pulse_channel_1;
    PulseChannel pulse_channel_2;
    TriangleChannel triangle_channel;
    NoiseChannel noise_channel;

    int frameCounterCycle;
    Uint8 frameCounterMode;
};

#endif
