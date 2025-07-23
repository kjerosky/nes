#include "TriangleChannel.h"

const int TriangleChannel::WAVEFORM_VALUES[32] = {
    15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,  0,
    0,  1,  2,  3,  4,  5,   6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
};

// --------------------------------------------------------------------------

TriangleChannel::TriangleChannel()
:
is_length_counter_halted(true),
linear_counter_reload(0),
timer_reload(0),
length_counter(0),
waveform_index(0),
timer(0),
should_reload_linear_counter(false),
linear_counter(0) {

    // nothing to do for now
}

// --------------------------------------------------------------------------

TriangleChannel::~TriangleChannel() {
    // nothing to do for now
}

// --------------------------------------------------------------------------

void TriangleChannel::reset() {
    length_counter = 0;
    is_length_counter_halted = true;
}

// --------------------------------------------------------------------------

void TriangleChannel::set_length_counter_halted(bool value) {
    is_length_counter_halted = value;
}

// --------------------------------------------------------------------------

void TriangleChannel::set_linear_counter_reload(Uint8 value) {
    linear_counter_reload = value;
}

// --------------------------------------------------------------------------

void TriangleChannel::set_timer_reload_low_byte(Uint8 value) {
    timer_reload = (timer_reload & 0xFF00) | value;
}

// --------------------------------------------------------------------------

void TriangleChannel::set_timer_reload_high_byte(Uint8 value) {
    timer_reload = (timer_reload & 0x00FF) | (value << 8);
    should_reload_linear_counter = true;
}

// --------------------------------------------------------------------------

void TriangleChannel::set_length_counter(Uint8 value) {
    length_counter = value;
}

// --------------------------------------------------------------------------

void TriangleChannel::clock_timer() {
    if (timer == 0) {
        timer = timer_reload;
        waveform_index = (waveform_index + 1) % 32;
    } else {
        timer--;
    }
}

// --------------------------------------------------------------------------

void TriangleChannel::clock_quarter_frame() {
    if (should_reload_linear_counter) {
        linear_counter = linear_counter_reload;
    } else if (linear_counter > 0) {
        linear_counter--;
    }

    if (!is_length_counter_halted) {
        should_reload_linear_counter = false;
    }
}

// --------------------------------------------------------------------------

void TriangleChannel::clock_half_frame() {
    if (length_counter > 0 && !is_length_counter_halted) {
        length_counter--;
    }
}

// --------------------------------------------------------------------------

int TriangleChannel::get_output() {
    int output = 0;

    if (linear_counter > 0 && length_counter > 0) {
        output = WAVEFORM_VALUES[waveform_index];
    }

    return output;
}
