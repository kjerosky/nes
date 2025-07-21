#ifndef MAPPER003_H
#define MAPPER003_H

#include "Mapper.h"

class Mapper003 : public Mapper {

public:

    Mapper003(unsigned char programRomBankCount, unsigned char characterRomBankCount);
    ~Mapper003();

    void reset() override;

    bool mapCpuRead(nesWord requestedAddress, unsigned int& mappedAddress) override;
    bool mapCpuWrite(nesWord requestedAddress, unsigned int& mappedAddress, nesByte data) override;
    bool mapPpuRead(nesWord requestedAddress, unsigned int& mappedAddress) override;
    bool mapPpuWrite(nesWord requestedAddress, unsigned int& mappedAddress) override;

private:

    int selected_chr_bank;
};

#endif
