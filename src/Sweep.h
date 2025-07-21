#ifndef SWEEP_H
#define SWEEP_H

#include <SDL3/SDL.h>

class Sweep {

public:

    Sweep(bool is_for_pulse_2);
    ~Sweep();

    void set_values(bool is_enabled, Uint8 divider_period, bool is_negated, Uint8 shift);
    void clock(Uint16& timer);
    bool is_muted();

private:

    bool is_for_pulse_2;
    bool is_enabled;
    Uint8 divider_period;
    bool is_negated;
    Uint8 shift;

    bool reload;
    Uint8 divider_counter;
    bool muted;
};

#endif
