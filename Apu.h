#ifndef APU_H
#define APU_H

#include "Types.h"

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

    float fastSin(float value);
    float samplePulse(bool enabled, nesWord timer, float dutyCycle);

    double globalTime;
    int cycleCount;

    bool pulse1Enabled;
    nesWord pulse1Timer;
    float pulse1DutyCycle;
    float pulse1Output;
};

#endif
