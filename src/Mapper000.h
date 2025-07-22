#ifndef MAPPER000_H
#define MAPPER000_H

#include <SDL3/SDL.h>

#include "Mapper.h"

class Mapper000 : public Mapper {

public:

    Mapper000(unsigned char programRomBankCount, unsigned char characterRomBankCount);
    ~Mapper000();

    bool mapCpuRead(Uint16 requestedAddress, unsigned int& mappedAddress) override;
    bool mapCpuWrite(Uint16 requestedAddress, unsigned int& mappedAddress, Uint8 data) override;
    bool mapPpuRead(Uint16 requestedAddress, unsigned int& mappedAddress) override;
    bool mapPpuWrite(Uint16 requestedAddress, unsigned int& mappedAddress) override;
};

#endif
