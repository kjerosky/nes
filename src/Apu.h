#ifndef APU_H
#define APU_H

#include <vector>

#include "Types.h"
#include "PulseChannel.h"

class Apu {

public:

    Apu();
    ~Apu();

    void reset();
    void clockTick();
    float getAudioSampleOutput();

    nesByte cpuRead(nesWord address, bool onlyRead = false);
    void cpuWrite(nesWord address, nesByte data);

private:

    std::vector<nesByte> lengthsTable;

    int cycleCount;

    PulseChannel pulse_channel_1;
    PulseChannel pulse_channel_2;

    int frameCounterCycle;
    nesByte frameCounterMode;
};

#endif
