#include "Apu.h"
#include <math.h>

#define PI 3.14159f

Apu::Apu()
:
pulse_channel_1(false),
pulse_channel_2(true) {

    lengthsTable = std::vector<nesByte>(0x20);
    lengthsTable[0x00] = 10;
    lengthsTable[0x01] = 254;
    lengthsTable[0x02] = 20;
    lengthsTable[0x03] = 2;
    lengthsTable[0x04] = 40;
    lengthsTable[0x05] = 4;
    lengthsTable[0x06] = 80;
    lengthsTable[0x07] = 6;
    lengthsTable[0x08] = 160;
    lengthsTable[0x09] = 8;
    lengthsTable[0x0A] = 60;
    lengthsTable[0x0B] = 10;
    lengthsTable[0x0C] = 14;
    lengthsTable[0x0D] = 12;
    lengthsTable[0x0E] = 26;
    lengthsTable[0x0F] = 14;
    lengthsTable[0x10] = 12;
    lengthsTable[0x11] = 16;
    lengthsTable[0x12] = 24;
    lengthsTable[0x13] = 18;
    lengthsTable[0x14] = 48;
    lengthsTable[0x15] = 20;
    lengthsTable[0x16] = 96;
    lengthsTable[0x17] = 22;
    lengthsTable[0x18] = 192;
    lengthsTable[0x19] = 24;
    lengthsTable[0x1A] = 72;
    lengthsTable[0x1B] = 26;
    lengthsTable[0x1C] = 16;
    lengthsTable[0x1D] = 28;
    lengthsTable[0x1E] = 32;
    lengthsTable[0x1F] = 30;

    reset();
}

Apu::~Apu() {
    reset();
}

void Apu::reset() {
    cycleCount = 0;

    pulse_channel_1.reset();
    pulse_channel_2.reset();

    frameCounterCycle = 0;
    frameCounterMode = 0;
}

void Apu::clockTick() {
    if (cycleCount % 6 == 0) {
        cycleCount = 0;

        pulse_channel_1.clock_timer();
        pulse_channel_2.clock_timer();

        bool onQuarterFrame = false;
        bool onHalfFrame = false;
        if (frameCounterCycle == 3729) {
            onQuarterFrame = true;
        } else if (frameCounterCycle == 7457) {
            onQuarterFrame = true;
            onHalfFrame = true;
        } else if (frameCounterCycle == 11186) {
            onQuarterFrame = true;
        } else if ((frameCounterMode == 0 && frameCounterCycle == 14915) || (frameCounterMode == 1 && frameCounterCycle == 18641)) {
            onQuarterFrame = true;
            onHalfFrame = true;
            frameCounterCycle = 0;
        }

        if (onQuarterFrame) {
            pulse_channel_1.clock_quarter_frame();
            pulse_channel_2.clock_quarter_frame();
        }

        if (onHalfFrame) {
            pulse_channel_1.clock_half_frame();
            pulse_channel_2.clock_half_frame();
        }

        frameCounterCycle++;
    }

    cycleCount++;
}

float Apu::getAudioSampleOutput() {
    // Here, we'll perform audio mixing according to blargg's formulas.

    int pulse1 = pulse_channel_1.get_output();
    int pulse2 = pulse_channel_2.get_output();
    double pulse_out = 0.0;
    if (pulse1 != 0 || pulse2 != 0) {
        pulse_out = 95.88 / ((8128.0 / (pulse1 + pulse2)) + 100);
    }

    int triangle = 0; //todo
    int noise = 0; //todo
    int dmc = 0; //todo
    double tnd_out = 0.0;
    if (triangle != 0 || noise != 0 || dmc != 0) {
        tnd_out = 159.79 / (1.0 / (triangle / 8227.0 + noise / 12241.0 + dmc / 22638.0) + 100);
    }

    return (pulse_out + tnd_out);
}

nesByte Apu::cpuRead(nesWord address, bool onlyRead) {
    nesByte value = 0x00;

    if (address == 0x4015) {
        //TODO IMPLEMENT THIS
    }

    return value;
}

void Apu::cpuWrite(nesWord address, nesByte data) {
    switch (address) {

        // =============== PULSE 1 ===============

        // pulse 1
        // Duty (D), envelope loop / length counter halt (L), constant volume (C), volume/envelope (V)
        // DDLC VVVV
        case 0x4000:
            pulse_channel_1.set_duty_cycle(data >> 6);
            pulse_channel_1.set_length_counter_halted((data & 0x20) != 0);
            pulse_channel_1.set_envelope_volume_values((data & 0x10) != 0, data & 0x0F);
            pulse_channel_1.restart_envelope();
            break;

        // pulse 1
        // Sweep unit: enabled (E), period (P), negate (N), shift (S)
        // EPPP NSSS
        case 0x4001:
            pulse_channel_1.set_sweep_values(
                (data & 0x80) != 0,
                (data >> 4) & 0x07,
                (data & 0x08) != 0,
                data & 0x07
            );
            break;

        // pulse 1
        // Timer low (T)
        // TTTT TTTT
        case 0x4002:
            pulse_channel_1.set_timer_lower_byte(data);
            break;

        // pulse 1
        // Length counter load (L), timer high (T)
        // LLLL LTTT
        case 0x4003:
            pulse_channel_1.set_timer_upper_byte(data & 0x07);
            pulse_channel_1.set_length_counter(lengthsTable[data >> 3]);
            pulse_channel_1.restart_envelope();
            break;

        // =============== PULSE 2 ===============

        // pulse 2
        // Duty (D), envelope loop / length counter halt (L), constant volume (C), volume/envelope (V)
        // DDLC VVVV
        case 0x4004:
            pulse_channel_2.set_duty_cycle(data >> 6);
            pulse_channel_2.set_length_counter_halted((data & 0x20) != 0);
            pulse_channel_2.set_envelope_volume_values((data & 0x10) != 0, data & 0x0F);
            pulse_channel_2.restart_envelope();
            break;

        // pulse 2
        // Sweep unit: enabled (E), period (P), negate (N), shift (S)
        // EPPP NSSS
        case 0x4005:
            pulse_channel_2.set_sweep_values(
                (data & 0x80) != 0,
                (data >> 4) & 0x07,
                (data & 0x08) != 0,
                data & 0x07
            );
            break;

        // pulse 2
        // Timer low (T)
        // TTTT TTTT
        case 0x4006:
            pulse_channel_2.set_timer_lower_byte(data);
            break;

        // pulse 2
        // Length counter load (L), timer high (T)
        // LLLL LTTT
        case 0x4007:
            pulse_channel_2.set_timer_upper_byte(data & 0x07);
            pulse_channel_2.set_length_counter(lengthsTable[data >> 3]);
            pulse_channel_2.restart_envelope();
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
            pulse_channel_1.set_enabled((data & 0x01) != 0);
            pulse_channel_2.set_enabled((data & 0x02) != 0);
            //todo add others here!!!
            break;

        // =============== FRAME COUNTER ===============

        // frame counter
        // Mode (M, 0 = 4-step, 1 = 5-step), IRQ inhibit flag (I)
        // MI-- ----
        case 0x4017:
            frameCounterMode = (data & 0x80) ? 1 : 0;
            //TODO IRQ INHIBIT FLAG
            break;
    }
}
