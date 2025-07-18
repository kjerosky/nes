#include "Sweep.h"

// --------------------------------------------------------------------------

Sweep::Sweep(bool is_for_pulse_2)
:
is_for_pulse_2(is_for_pulse_2),
is_enabled(false),
divider_period(0),
is_negated(false),
shift(0),
reload(false),
divider_counter(0) {

    // nothing to do for now
}

// --------------------------------------------------------------------------

Sweep::~Sweep() {
    // nothing to do for now
}

// --------------------------------------------------------------------------

void Sweep::set_values(bool is_enabled, Uint8 divider_period, bool is_negated, Uint8 shift) {
    this->is_enabled = is_enabled;
    this->divider_period = divider_period;
    this->is_negated = is_negated;
    this->shift = shift;

    reload = true;
}

// --------------------------------------------------------------------------

void Sweep::clock(Uint16& timer) {
    Uint16 change = timer >> shift;

    Uint16 target_timer;
    if (is_negated) {
        if (is_for_pulse_2) {
            target_timer = timer - change - 1;
        } else {
            target_timer = timer - change;
        }
    } else {
        target_timer = timer + change;
    }

    bool is_muted;
    if (target_timer < 8 || target_timer > 0x07FF) {
        is_muted = true;
    } else {
        is_muted = false;
    }

    if (reload) {
        divider_counter = divider_period + 1;
        reload = false;
        return;
    }

    if (divider_counter > 0) {
        divider_counter--;
    }

    if (divider_counter == 0) {
        divider_counter = divider_period + 1;

        if (is_enabled && shift > 0 && !is_muted) {
            timer = target_timer;
        }
    }
}
