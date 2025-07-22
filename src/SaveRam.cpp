#include "SaveRam.h"

#include <algorithm>

// --------------------------------------------------------------------------

SaveRam::SaveRam() {
    clear();
}

// --------------------------------------------------------------------------

SaveRam::~SaveRam() {
    // nothing to do for now
}

// --------------------------------------------------------------------------

void SaveRam::clear() {
    std::fill(contents, contents + CONTENTS_SIZE, 0);
}

// --------------------------------------------------------------------------

Uint8 SaveRam::read(Uint16 address) {
    return contents[address & 0x1FFF];
}

// --------------------------------------------------------------------------

void SaveRam::write(Uint16 address, Uint8 value) {
    contents[address & 0x1FFF] = value;
}
