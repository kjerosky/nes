#ifndef PULSE_CHANNEL_H
#define PULSE_CHANNEL_H

#include <SDL3/SDL.h>

#include "Envelope.h"

class PulseChannel {

public:

    PulseChannel();
    ~PulseChannel();

    void reset();
    void clock_quarter_frame();
    void clock_half_frame();
    void set_enabled(bool is_enabled);
    void set_timer_upper_byte(Uint8 upper_byte);
    void set_timer_lower_byte(Uint8 lower_byte);
    void set_duty_cycle(Uint8 right_shifted_duty_cycle_bits);
    void set_length_counter(Uint8 length_counter);
    void set_length_counter_halted(bool is_length_counter_halted);
    void set_envelope_volume_values(bool use_constant_volume, Uint8 divider_period);
    void restart_envelope();
    float sample(double global_time);

private:

    bool is_enabled;
    Uint16 timer;
    Uint8 length_counter;
    bool is_length_counter_halted;
    float duty_cycle;

    Envelope envelope;

    float fast_sin(float value);
};

#endif
