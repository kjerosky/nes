#include "Apu.h"
#include <math.h>

#define PI 3.14159f

Apu::Apu() {
    reset();
}

Apu::~Apu() {
    reset();
}

void Apu::reset() {
    globalTime = 0;
    cycleCount = 0;

    pulse1Enabled = false;
    pulse1Timer = 0;
    pulse1DutyCycle = 0;
    pulse1Output = 0;
}

void Apu::clockTick() {
    // In this implementation, we're clocking the apu at the same rate as the ppu,
    // so we track time passing according to the ppu clock cycle time.
    globalTime += 1.0 / 5369318;

    if (cycleCount % 6 == 0) {
        cycleCount = 0;

        pulse1Output = samplePulse(pulse1Enabled, pulse1Timer, pulse1DutyCycle);
    }

    cycleCount++;
}

float Apu::getAudioSampleOutput() {
    //TODO IMPLEMENT CHANNEL MIXING
    return pulse1Output;
}

nesByte Apu::cpuRead(nesWord address, bool onlyRead) {
    //TODO IMPLEMENT THIS
    return 0x00;
}

void Apu::cpuWrite(nesWord address, nesByte data) {
    nesByte dutyCycleBits;

    switch (address) {

        // =============== PULSE 1 ===============

        // pulse 1
        // Duty (D), envelope loop / length counter halt (L), constant volume (C), volume/envelope (V)
        // DDLC VVVV
        case 0x4000:
            dutyCycleBits = data >> 6;
            switch (dutyCycleBits) {
                case 0x00: pulse1DutyCycle = 0.125f; break;
                case 0x01: pulse1DutyCycle = 0.250f; break;
                case 0x02: pulse1DutyCycle = 0.500f; break;
                case 0x03: pulse1DutyCycle = 0.750f; break;
            }
            break;

        // pulse 1
        // Sweep unit: enabled (E), period (P), negate (N), shift (S)
        // EPPP NSSS
        case 0x4001:
            //TODO
            break;

        // pulse 1
        // Timer low (T)
        // TTTT TTTT
        case 0x4002:
            pulse1Timer = (pulse1Timer & 0xFF00) | data;
            break;

        // pulse 1
        // Length counter load (L), timer high (T)
        // LLLL LTTT
        case 0x4003:
            pulse1Timer = ((data & 0x07) << 8) | (pulse1Timer & 0x00FF);
            break;

        // =============== PULSE 2 ===============

        // pulse 2
        // Duty (D), envelope loop / length counter halt (L), constant volume (C), volume/envelope (V)
        // DDLC VVVV
        case 0x4004:
            //TODO
            break;

        // pulse 2
        // Sweep unit: enabled (E), period (P), negate (N), shift (S)
        // EPPP NSSS
        case 0x4005:
            //TODO
            break;

        // pulse 2
        // Timer low (T)
        // TTTT TTTT
        case 0x4006:
            //TODO
            break;

        // pulse 2
        // Length counter load (L), timer high (T)
        // LLLL LTTT
        case 0x4007:
            //TODO
            break;

        // =============== TRIANGLE ===============

        // triangle
        // Length counter halt / linear counter control (C), linear counter load (R)
        // CRRR RRRR
        case 0x4008:
            //TODO
            break;

        // UNUSED
        case 0x4009:
            // do nothing
            break;

        // triangle
        // Timer low (T)
        // TTTT TTTT
        case 0x400A:
            //TODO
            break;

        // triangle
        // Length counter load (L), timer high (T)
        // LLLL LTTT
        case 0x400B:
            //TODO
            break;

        // =============== NOISE ===============

        // noise
        // Envelope loop / length counter halt (L), constant volume (C), volume/envelope (V)
        // --LC VVVV
        case 0x400C:
            //TODO
            break;

        // UNUSED
        case 0x400D:
            // do nothing
            break;

        // noise
        // Loop noise (L), noise period (P)
        // L--- PPPP
        case 0x400E:
            //TODO
            break;

        // noise
        // Length counter load (L)
        // LLLL L---
        case 0x400F:
            //TODO
            break;

        // =============== DMC ===============

        // dmc
        // IRQ enable (I), loop (L), frequency (R)
        // IL-- RRRR
        case 0x4010:
            //TODO
            break;

        // dmc
        // Load counter (D)
        // -DDD DDDD
        case 0x4011:
            //TODO
            break;

        // dmc
        // Sample address (A)
        // AAAA AAAA
        case 0x4012:
            //TODO
            break;

        // dmc
        // Sample length (L)
        // LLLL LLLL
        case 0x4013:
            //TODO
            break;

        // =============== STATUS ===============

        // status
        // Enable DMC (D), noise (N), triangle (T), and pulse channels (2/1)
        // ---D NT21
        case 0x4015:

            //TODO
            break;

        // =============== FRAME COUNTER ===============

        // frame counter
        // Mode (M, 0 = 4-step, 1 = 5-step), IRQ inhibit flag (I)
        // MI-- ----
        case 0x4017:
            //TODO
            break;
    }
}

float Apu::fastSin(float value) {
    float j = value * 0.15915;
    j = j - (int)j;
    return 20.785 * j * (j - 0.5) * (j - 1.0f);
}

float Apu::samplePulse(bool enabled, nesWord timer, float dutyCycle) {
    float a = 0;
    float b = 0;

    int harmonics = 20;
    double frequency = 1789773.0 / (16 * (double)(timer + 1));
    float p = dutyCycle * 2.0f * PI;

    for (float n = 1; n < harmonics; n++) {
        float c = n * frequency * 2.0f * PI * globalTime;
        a += fastSin(c) / n;
        b += fastSin(c - p * n) / n;
    }

    return 2.0f / PI * (a - b);
}
