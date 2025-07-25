#include "Mapper000.h"

Mapper000::Mapper000(unsigned char programRomBankCount, unsigned char characterRomBankCount)
: Mapper(programRomBankCount, characterRomBankCount) {
    // do nothing extra
}

Mapper000::~Mapper000() {
    // do nothing
}

bool Mapper000::mapCpuRead(Uint16 requestedAddress, unsigned int& mappedAddress) {
    mappedAddress = requestedAddress & (programRomBankCount > 1 ? 0x7FFF : 0x3FFF);
    return true;
}

bool Mapper000::mapCpuWrite(Uint16 requestedAddress, unsigned int& mappedAddress, Uint8 data) {
    mappedAddress = requestedAddress & (programRomBankCount > 1 ? 0x7FFF : 0x3FFF);
    return true;
}

bool Mapper000::mapPpuRead(Uint16 requestedAddress, unsigned int& mappedAddress) {
    // no mapping necessary
    mappedAddress = requestedAddress;
    return true;
}

bool Mapper000::mapPpuWrite(Uint16 requestedAddress, unsigned int& mappedAddress) {
    // If the CHR-ROM is actually CHR-RAM, we'll allow writes to it.
    if (characterRomBankCount == 0) {
        mappedAddress = requestedAddress;
        return true;
    }

    return false;
}

bool Mapper000::handles_mirror(Mirroring& mirroring) {
    return false;
}
