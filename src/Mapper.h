#ifndef MAPPER_H
#define MAPPER_H

#include "Types.h"

class Mapper {

public:

    Mapper(unsigned char programRomBankCount, unsigned char characterRomBankCount);
    virtual ~Mapper();

    virtual void reset();

    virtual bool mapCpuRead(nesWord requestedAddress, unsigned int& mappedAddress) = 0;
    virtual bool mapCpuWrite(nesWord requestedAddress, unsigned int& mappedAddress, nesByte data) = 0;
    virtual bool mapPpuRead(nesWord requestedAddress, unsigned int& mappedAddress) = 0;
    virtual bool mapPpuWrite(nesWord requestedAddress, unsigned int& mappedAddress) = 0;

protected:

    unsigned char programRomBankCount;
    unsigned char characterRomBankCount;
};

#endif
