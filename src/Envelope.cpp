#include "Envelope.h"

// --------------------------------------------------------------------------

Envelope::Envelope()
:
is_looping(false),
use_constant_volume(false),
divider_period(0),
start(false),
divider_counter(0),
decay_counter(0) {

    // nothing to do for now
}

// --------------------------------------------------------------------------

Envelope::~Envelope() {
    // nothing to do for now
}

// --------------------------------------------------------------------------

void Envelope::set_looping(bool is_looping) {
    this->is_looping = is_looping;
}

// --------------------------------------------------------------------------

void Envelope::set_volume_values(bool use_constant_volume, Uint8 divider_period) {
    this->use_constant_volume = use_constant_volume;
    this->divider_period = divider_period;
}

// --------------------------------------------------------------------------

void Envelope::clock() {
    if (start) {
        decay_counter = 15;
        divider_counter = divider_period;
        start = false;
    } else {
        if (divider_counter == 0) {
            divider_counter = divider_period;
            if (decay_counter > 0) {
                decay_counter--;
            } else if (is_looping) {
                decay_counter = 15;
            }
        } else {
            divider_counter--;
        }
    }

}

// --------------------------------------------------------------------------

Uint8 Envelope::get_volume() {
    return use_constant_volume ? divider_period : decay_counter;
}

// --------------------------------------------------------------------------

void Envelope::restart() {
    start = true;
}
