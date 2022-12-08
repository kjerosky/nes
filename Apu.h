#ifndef APU_H
#define APU_H

class Apu {

public:

    Apu();
    ~Apu();

    void reset();
    void clockTick();
    float getAudioSampleOutput();

private:

    double globalTime;
};

#endif
