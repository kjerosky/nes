#ifndef MAPPER002_H
#define MAPPER002_H

#include "Mapper.h"

class Mapper002 : public Mapper {

public:

    Mapper002(unsigned char programRomBankCount, unsigned char characterRomBankCount);
    ~Mapper002();

    void reset() override;

    bool mapCpuRead(Uint16 requestedAddress, unsigned int& mappedAddress) override;
    bool mapCpuWrite(Uint16 requestedAddress, unsigned int& mappedAddress, Uint8 data) override;
    bool mapPpuRead(Uint16 requestedAddress, unsigned int& mappedAddress) override;
    bool mapPpuWrite(Uint16 requestedAddress, unsigned int& mappedAddress) override;

private:

    int switchableBankIndex;
    int fixedBankIndex;
};

#endif
