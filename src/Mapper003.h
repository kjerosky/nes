#ifndef MAPPER003_H
#define MAPPER003_H

#include "Mapper.h"

class Mapper003 : public Mapper {

public:

    Mapper003(unsigned char programRomBankCount, unsigned char characterRomBankCount);
    ~Mapper003();

    void reset() override;

    bool mapCpuRead(Uint16 requestedAddress, unsigned int& mappedAddress) override;
    bool mapCpuWrite(Uint16 requestedAddress, unsigned int& mappedAddress, Uint8 data) override;
    bool mapPpuRead(Uint16 requestedAddress, unsigned int& mappedAddress) override;
    bool mapPpuWrite(Uint16 requestedAddress, unsigned int& mappedAddress) override;

private:

    int selected_chr_bank;
};

#endif
