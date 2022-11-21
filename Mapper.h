#ifndef MAPPER_H
#define MAPPER_H

#include "Types.h"

class Mapper {

public:

    Mapper(unsigned char programRomBankCount, unsigned char characterRomBankCount);
    virtual ~Mapper();

    virtual bool mapCpuRead(nesWord requestedAddress, nesWord& mappedAddress) = 0;
    virtual bool mapCpuWrite(nesWord requestedAddress, nesWord& mappedAddress) = 0;
    virtual bool mapPpuRead(nesWord requestedAddress, nesWord& mappedAddress) = 0;
    virtual bool mapPpuWrite(nesWord requestedAddress, nesWord& mappedAddress) = 0;

protected:

    unsigned char programRomBankCount;
    unsigned char characterRomBankCount;
};

#endif
