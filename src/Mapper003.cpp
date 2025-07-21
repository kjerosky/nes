#include "Mapper003.h"

Mapper003::Mapper003(unsigned char programRomBankCount, unsigned char characterRomBankCount)
:
Mapper(programRomBankCount, characterRomBankCount) {

    reset();
}

// --------------------------------------------------------------------------

Mapper003::~Mapper003() {
    // nothing to do for now
}

// --------------------------------------------------------------------------

void Mapper003::reset() {
    selected_chr_bank = 0;
}

// --------------------------------------------------------------------------

bool Mapper003::mapCpuRead(nesWord requestedAddress, unsigned int& mappedAddress) {
    mappedAddress = requestedAddress & (programRomBankCount > 1 ? 0x7FFF : 0x3FFF);
    return true;
}

// --------------------------------------------------------------------------

bool Mapper003::mapCpuWrite(nesWord requestedAddress, unsigned int& mappedAddress, nesByte data) {
    selected_chr_bank = data % characterRomBankCount;
    return false;
}

// --------------------------------------------------------------------------

bool Mapper003::mapPpuRead(nesWord requestedAddress, unsigned int& mappedAddress) {
    mappedAddress = selected_chr_bank * 0x2000 + requestedAddress;
    return true;
}

// --------------------------------------------------------------------------

bool Mapper003::mapPpuWrite(nesWord requestedAddress, unsigned int& mappedAddress) {
    return false;
}
