#ifndef MAPPER_H
#define MAPPER_H

#include <SDL3/SDL.h>

class Mapper {

public:

    Mapper(unsigned char programRomBankCount, unsigned char characterRomBankCount);
    virtual ~Mapper();

    virtual void reset();

    virtual bool mapCpuRead(Uint16 requestedAddress, unsigned int& mappedAddress) = 0;
    virtual bool mapCpuWrite(Uint16 requestedAddress, unsigned int& mappedAddress, Uint8 data) = 0;
    virtual bool mapPpuRead(Uint16 requestedAddress, unsigned int& mappedAddress) = 0;
    virtual bool mapPpuWrite(Uint16 requestedAddress, unsigned int& mappedAddress) = 0;

protected:

    unsigned char programRomBankCount;
    unsigned char characterRomBankCount;
};

#endif
