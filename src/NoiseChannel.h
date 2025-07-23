#ifndef NOISE_CHANNEL_H
#define NOISE_CHANNEL_H

#include <SDL3/SDL.h>

#include "Envelope.h"

class NoiseChannel {

public:

    NoiseChannel();
    ~NoiseChannel();

    void set_length_counter_halted(bool value);
    void set_envelope_volume_values(bool use_constant_volume, Uint8 divider_period);
    void set_mode(bool value);
    void set_timer_reload_by_index(Uint8 index);
    void set_length_counter(Uint8 value);
    void clock_timer();
    void clock_quarter_frame();
    void clock_half_frame();
    void reset();
    int get_output();

private:

    static const Uint16 TIMER_RELOAD_VALUES[16];

    bool is_length_counter_halted;
    bool mode;
    Uint16 timer_reload;

    Envelope envelope;
    Uint8 length_counter;
    Uint16 timer;
    Uint16 lfsr;

    void clock_lfsr();
};

#endif
