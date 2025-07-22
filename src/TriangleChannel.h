#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <SDL3/SDL.h>

class TriangleChannel {

public:

    TriangleChannel();
    ~TriangleChannel();

    void set_control(bool value);
    void set_linear_counter_reload(Uint8 value);
    void set_timer_reload_low_byte(Uint8 value);
    void set_timer_reload_high_byte(Uint8 value);
    void set_length_counter(Uint8 value);
    void clock_timer();
    void clock_quarter_frame();
    void clock_half_frame();
    int get_output();

private:

    static const int WAVEFORM_VALUES[32];

    bool control;
    Uint8 linear_counter_reload;
    Uint16 timer_reload;
    Uint8 length_counter;

    Uint8 waveform_index;
    Uint16 timer;
    bool should_reload_linear_counter;
    Uint8 linear_counter;
};

#endif
