#ifndef MAPPER_H
#define MAPPER_H

#include "Types.h"

class Mapper {

public:

    Mapper(unsigned char programRomBankCount, unsigned char characterRomBankCount);
    virtual ~Mapper();

    virtual nesWord mapCpuRead(nesWord requestedAddress) = 0;
    virtual nesWord mapCpuWrite(nesWord requestedAddress) = 0;
    virtual nesWord mapPpuRead(nesWord requestedAddress) = 0;
    virtual nesWord mapPpuWrite(nesWord requestedAddress) = 0;

protected:

    unsigned char programRomBankCount;
    unsigned char characterRomBankCount;
};

#endif
