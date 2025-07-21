#ifndef PULSE_CHANNEL_H
#define PULSE_CHANNEL_H

#include <SDL3/SDL.h>

#include "Envelope.h"
#include "Sweep.h"

class PulseChannel {

public:

    PulseChannel(bool is_pulse_2);
    ~PulseChannel();

    void reset();
    void clock_quarter_frame();
    void clock_half_frame();
    void set_enabled(bool is_enabled);
    void set_timer_upper_byte(Uint8 upper_byte);
    void set_timer_lower_byte(Uint8 lower_byte);
    void set_duty_cycle(Uint8 duty_cycle);
    void set_length_counter(Uint8 length_counter);
    void set_length_counter_halted(bool is_length_counter_halted);
    void set_envelope_volume_values(bool use_constant_volume, Uint8 divider_period);
    void set_sweep_values(bool is_enabled, Uint8 divider_period, bool is_negated, Uint8 shift);
    void restart_envelope();
    void clock_timer();
    int get_output();

private:

    static const Uint8 DUTY_TABLE[4][8];

    bool is_enabled;
    Uint16 timer;
    Uint8 length_counter;
    bool is_length_counter_halted;
    Uint8 duty_cycle;
    Uint8 duty_cycle_index;
    int timer_counter;

    Envelope envelope;
    Sweep sweep;
};

#endif
