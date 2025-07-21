#include "PulseChannel.h"

const Uint8 PulseChannel::DUTY_TABLE[4][8] = {
    { 0, 1, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 0, 0, 0, 0, 0 },
    { 0, 1, 1, 1, 1, 0, 0, 0 },
    { 1, 0, 0, 1, 1, 1, 1, 1 },
};

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
    timer_counter = 1;

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
    duty_cycle_index = 0;
}

// --------------------------------------------------------------------------

void PulseChannel::set_timer_lower_byte(Uint8 lower_byte) {
    timer = (timer & 0xFF00) | lower_byte;
}

// --------------------------------------------------------------------------

void PulseChannel::set_duty_cycle(Uint8 duty_cycle) {
    this->duty_cycle = duty_cycle;
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

int PulseChannel::get_output() {
    int output;
    if (length_counter == 0) {
        output = 0;
    } else if (sweep.is_muted()) {
        output = 0;
    } else if (timer < 8) {
        output = 0;
    } else {
        output = DUTY_TABLE[duty_cycle][duty_cycle_index];
    }

    return output * envelope.get_volume();
}

// --------------------------------------------------------------------------

void PulseChannel::clock_timer() {
    if (timer_counter == 0) {
        timer_counter = timer;
        duty_cycle_index = (duty_cycle_index + 1) % 8;
    } else {
        timer_counter--;
    }
}
