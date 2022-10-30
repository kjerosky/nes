#include "Mapper000.h"

Mapper000::Mapper000(unsigned char programRomBankCount, unsigned char characterRomBankCount)
: Mapper(programRomBankCount, characterRomBankCount) {
    // do nothing
}

Mapper000::~Mapper000() {
    // do nothing
}

nesWord Mapper000::mapCpuRead(nesWord requestedAddress) {
    return requestedAddress & (programRomBankCount > 1 ? 0x7FFF : 0x3FFF);
}

nesWord Mapper000::mapCpuWrite(nesWord requestedAddress) {
    return requestedAddress & (programRomBankCount > 1 ? 0x7FFF : 0x3FFF);
}

nesWord Mapper000::mapPpuRead(nesWord requestedAddress) {
    // no mapping necessary
    return requestedAddress;
}

nesWord Mapper000::mapPpuWrite(nesWord requestedAddress) {
    // no mapping necessary
    return requestedAddress;
}
