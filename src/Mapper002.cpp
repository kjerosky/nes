#include "Mapper002.h"

Mapper002::Mapper002(unsigned char programRomBankCount, unsigned char characterRomBankCount)
: Mapper(programRomBankCount, characterRomBankCount) {
    // do nothing extra
}

Mapper002::~Mapper002() {
    // do nothing
}

void Mapper002::reset() {
    switchableBankIndex = 0;
    fixedBankIndex = programRomBankCount - 1;
}

bool Mapper002::mapCpuRead(Uint16 requestedAddress, unsigned int& mappedAddress) {
    if (requestedAddress >= 0x8000 && requestedAddress < 0xBFFF) {
        mappedAddress = 0x4000 * switchableBankIndex + (requestedAddress & 0x3FFF);
    } else {
        mappedAddress = 0x4000 * fixedBankIndex + (requestedAddress & 0x3FFF);
    }

    return true;
}

bool Mapper002::mapCpuWrite(Uint16 requestedAddress, unsigned int& mappedAddress, Uint8 data) {
    switchableBankIndex = data;
    return false;
}

bool Mapper002::mapPpuRead(Uint16 requestedAddress, unsigned int& mappedAddress) {
    // no mapping necessary
    mappedAddress = requestedAddress;
    return true;
}

bool Mapper002::mapPpuWrite(Uint16 requestedAddress, unsigned int& mappedAddress) {
    // If the CHR-ROM is actually CHR-RAM, we'll allow writes to it.
    if (characterRomBankCount == 0) {
        mappedAddress = requestedAddress;
        return true;
    }

    return false;
}
