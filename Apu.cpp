#include "Apu.h"
#include <math.h>

Apu::Apu() {
    reset();
}

Apu::~Apu() {
    // do nothing
}

void Apu::reset() {
    globalTime = 0;
}

void Apu::clockTick() {
    // In this implementation, we're clocking the apu at the same rate as the ppu,
    // so we track time passing according to the ppu clock cycle time.
    globalTime += 1.0 / 5369318;
}

float Apu::getAudioSampleOutput() {
    //TODO IMPLEMENT REAL SAMPLE OUTPUT
    return 0.5 * sin(globalTime * 440.0f * 2.0f * 3.14159f);
}
