#include "PulseChannel.h"

// --------------------------------------------------------------------------

PulseChannel::PulseChannel(bool is_pulse_2)
:
sweep(is_pulse_2) {

    reset();
}

// --------------------------------------------------------------------------

PulseChannel::~PulseChannel() {
    // nothing to do for now
}

// --------------------------------------------------------------------------

void PulseChannel::reset() {
    is_enabled = false;
    timer = 0;
    length_counter = 0;
    duty_cycle = 0.0f;
    set_length_counter_halted(true);

    envelope.set_volume_values(true, 0);
}

// --------------------------------------------------------------------------

void PulseChannel::clock_quarter_frame() {
    envelope.clock();
}

// --------------------------------------------------------------------------

void PulseChannel::clock_half_frame() {
    if (!is_enabled) {
        length_counter = 0;
    } else if (length_counter > 0 && !is_length_counter_halted) {
        length_counter--;
    }

    sweep.clock(timer);
}

// --------------------------------------------------------------------------

void PulseChannel::set_enabled(bool is_enabled) {
    this->is_enabled = is_enabled;
}

// --------------------------------------------------------------------------

void PulseChannel::set_timer_upper_byte(Uint8 upper_byte) {
    timer = (static_cast<Uint16>(upper_byte) << 8) | (timer & 0x00FF);
}

// --------------------------------------------------------------------------

void PulseChannel::set_timer_lower_byte(Uint8 lower_byte) {
    timer = (timer & 0xFF00) | lower_byte;
}

// --------------------------------------------------------------------------

void PulseChannel::set_duty_cycle(Uint8 right_shifted_duty_cycle_bits) {
    switch (right_shifted_duty_cycle_bits) {
        case 0x00: duty_cycle = 0.125f; break;
        case 0x01: duty_cycle = 0.250f; break;
        case 0x02: duty_cycle = 0.500f; break;
        case 0x03: duty_cycle = 0.750f; break;
    }
}

// --------------------------------------------------------------------------

void PulseChannel::set_length_counter(Uint8 length_counter) {
    this->length_counter = length_counter;
}

// --------------------------------------------------------------------------

void PulseChannel::set_length_counter_halted(bool is_length_counter_halted) {
    this->is_length_counter_halted = is_length_counter_halted;

    // The envelope loop and length counter halted flags share the same value.
    envelope.set_looping(is_length_counter_halted);
}

// --------------------------------------------------------------------------

void PulseChannel::set_envelope_volume_values(bool use_constant_volume, Uint8 divider_period) {
    envelope.set_volume_values(use_constant_volume, divider_period);
}

// --------------------------------------------------------------------------

void PulseChannel::set_sweep_values(bool is_enabled, Uint8 divider_period, bool is_negated, Uint8 shift) {
    sweep.set_values(is_enabled, divider_period, is_negated, shift);
}

// --------------------------------------------------------------------------

void PulseChannel::restart_envelope() {
    envelope.restart();
}

// --------------------------------------------------------------------------

float PulseChannel::sample(double global_time) {
    float output = 0.0f;

    if (is_enabled && length_counter > 0 && timer >= 8) {
        float a = 0;
        float b = 0;

        int harmonics = 20;
        double frequency = 1789773.0 / (16 * (double)(timer + 1));
        float p = duty_cycle * 2.0f * SDL_PI_F;

        for (float n = 1; n < harmonics; n++) {
            float c = n * frequency * 2.0f * SDL_PI_F * global_time;
            a += fast_sin(c) / n;
            b += fast_sin(c - p * n) / n;
        }

        output = 2.0f / SDL_PI_F * (a - b);
    }

    float volume_scale = envelope.get_volume() / 16.0f;

    return output * volume_scale;
}

// --------------------------------------------------------------------------

float PulseChannel::fast_sin(float value) {
    float j = value * 0.15915;
    j = j - (int)j;
    return 20.785 * j * (j - 0.5) * (j - 1.0f);
}
