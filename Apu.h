#ifndef APU_H
#define APU_H

#include "Types.h"
#include <vector>

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

    float fastSin(float value);
    float samplePulse(bool enabled, nesWord timer, float dutyCycle);

    double globalTime;
    int cycleCount;

    bool pulse1Enabled;
    nesWord pulse1Timer;
    float pulse1DutyCycle;
    float pulse1Output;
    nesByte pulse1LengthCounter;
    bool pulse1LengthCounterHalted;

    int frameCounterCycle;
    nesByte frameCounterMode;
};

#endif
