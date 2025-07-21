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
divider_counter(0),
muted(true) {

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
            if (change > timer) {
                target_timer = 0;
            } else {
                target_timer = timer - change;
            }
        } else {
            if (change + 1 > timer) {
                target_timer = 0;
            } else {
                target_timer = timer - change - 1;
            }
        }
    } else {
        target_timer = timer + change;
    }

    if (target_timer > 0x07FF) {
        muted = true;
    } else {
        muted = false;
    }

    if (reload) {
        divider_counter = divider_period;
        reload = false;
        return;
    }

    if (divider_counter == 0) {
        divider_counter = divider_period;

        if (is_enabled && shift > 0 && !muted) {
            timer = target_timer;
        }
    } else {
        divider_counter--;
    }

}

// --------------------------------------------------------------------------

bool Sweep::is_muted() {
    return muted;
}
