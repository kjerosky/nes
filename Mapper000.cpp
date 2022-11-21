#include "Mapper000.h"

Mapper000::Mapper000(unsigned char programRomBankCount, unsigned char characterRomBankCount)
: Mapper(programRomBankCount, characterRomBankCount) {
    // do nothing
}

Mapper000::~Mapper000() {
    // do nothing
}

bool Mapper000::mapCpuRead(nesWord requestedAddress, nesWord& mappedAddress) {
    mappedAddress = requestedAddress & (programRomBankCount > 1 ? 0x7FFF : 0x3FFF);
    return true;
}

bool Mapper000::mapCpuWrite(nesWord requestedAddress, nesWord& mappedAddress) {
    mappedAddress = requestedAddress & (programRomBankCount > 1 ? 0x7FFF : 0x3FFF);
    return true;
}

bool Mapper000::mapPpuRead(nesWord requestedAddress, nesWord& mappedAddress) {
    // no mapping necessary
    mappedAddress = requestedAddress;
    return true;
}

bool Mapper000::mapPpuWrite(nesWord requestedAddress, nesWord& mappedAddress) {
    return false;
}
