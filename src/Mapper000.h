#ifndef MAPPER000_H
#define MAPPER000_H

#include "Mapper.h"
#include "Types.h"

class Mapper000 : public Mapper {

public:

    Mapper000(unsigned char programRomBankCount, unsigned char characterRomBankCount);
    ~Mapper000();

    bool mapCpuRead(nesWord requestedAddress, unsigned int& mappedAddress) override;
    bool mapCpuWrite(nesWord requestedAddress, unsigned int& mappedAddress, nesByte data) override;
    bool mapPpuRead(nesWord requestedAddress, unsigned int& mappedAddress) override;
    bool mapPpuWrite(nesWord requestedAddress, unsigned int& mappedAddress) override;
};

#endif
