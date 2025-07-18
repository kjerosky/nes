#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <SDL3/SDL.h>

class Envelope {

public:

    Envelope();
    ~Envelope();

    void set_looping(bool is_looping);
    void set_volume_values(bool use_constant_volume, Uint8 divider_period);
    void clock();
    Uint8 get_volume();
    void restart();

private:

    bool is_looping;
    bool use_constant_volume;
    Uint8 divider_period;

    bool start;
    Uint8 divider_counter;
    Uint8 decay_counter;
};

#endif
