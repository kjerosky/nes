#include "NoiseChannel.h"

const Uint16 NoiseChannel::TIMER_RELOAD_VALUES[16] = {
    4, 8, 16, 32, 64, 96, 128, 160, 202, 254, 380, 508, 762, 1016, 2034, 4068
};

// --------------------------------------------------------------------------

NoiseChannel::NoiseChannel()
:
mode(0),
timer_reload(0),
length_counter(0),
timer(0),
lfsr(0x0001) {

    set_length_counter_halted(true);
}

// --------------------------------------------------------------------------

NoiseChannel::~NoiseChannel() {
    // nothing to do for now
}

// --------------------------------------------------------------------------

void NoiseChannel::reset() {
    length_counter = 0;
    set_length_counter_halted(true);
    lfsr = 0x0001;
}

// --------------------------------------------------------------------------

void NoiseChannel::set_length_counter_halted(bool value) {
    is_length_counter_halted = value;

    // The envelope loop and length counter halted flags share the same value.
    envelope.set_looping(is_length_counter_halted);
}

// --------------------------------------------------------------------------

void NoiseChannel::set_envelope_volume_values(bool use_constant_volume, Uint8 divider_period) {
    envelope.set_volume_values(use_constant_volume, divider_period);
}

// --------------------------------------------------------------------------

void NoiseChannel::set_mode(bool value) {
    mode = value;
}

// --------------------------------------------------------------------------

void NoiseChannel::set_timer_reload_by_index(Uint8 index) {
    timer_reload = TIMER_RELOAD_VALUES[index];
}

// --------------------------------------------------------------------------

void NoiseChannel::set_length_counter(Uint8 value) {
    length_counter = value;
    envelope.restart();
}

// --------------------------------------------------------------------------

void NoiseChannel::clock_timer() {
    if (timer == 0) {
        timer = timer_reload;
        clock_lfsr();
    } else {
        timer--;
    }
}

// --------------------------------------------------------------------------

void NoiseChannel::clock_quarter_frame() {
    envelope.clock();
}

// --------------------------------------------------------------------------

void NoiseChannel::clock_half_frame() {
    if (!is_length_counter_halted && length_counter > 0) {
        length_counter--;
    }
}

// --------------------------------------------------------------------------

int NoiseChannel::get_output() {
    int output = 0;

    if (length_counter != 0 && (lfsr & 0x0001) == 0) {
        output = envelope.get_volume();
    }

    return output;
}

// --------------------------------------------------------------------------

void NoiseChannel::clock_lfsr() {
    Uint16 input_bit;
    if (mode) {
        input_bit = (lfsr & 0x0040) >> 6;
    } else {
        input_bit = (lfsr & 0x0002) >> 1;
    }

    Uint16 new_bit = (lfsr & 0x0001) ^ (input_bit);

    lfsr = (lfsr >> 1) | (new_bit << 14);
}
