#ifndef MAPPER000_H
#define MAPPER000_H

#include "Mapper.h"

class Mapper000 : public Mapper {

public:

    Mapper000(unsigned char programRomBankCount, unsigned char characterRomBankCount);
    ~Mapper000();

    nesWord mapCpuRead(nesWord requestedAddress) override;
    nesWord mapCpuWrite(nesWord requestedAddress) override;
    nesWord mapPpuRead(nesWord requestedAddress) override;
    nesWord mapPpuWrite(nesWord requestedAddress) override;
};

#endif
