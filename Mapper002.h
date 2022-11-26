#ifndef MAPPER002_H
#define MAPPER002_H

#include "Mapper.h"

class Mapper002 : public Mapper {

public:

    Mapper002(unsigned char programRomBankCount, unsigned char characterRomBankCount);
    ~Mapper002();

    void reset() override;

    bool mapCpuRead(nesWord requestedAddress, unsigned int& mappedAddress) override;
    bool mapCpuWrite(nesWord requestedAddress, unsigned int& mappedAddress, nesByte data) override;
    bool mapPpuRead(nesWord requestedAddress, unsigned int& mappedAddress) override;
    bool mapPpuWrite(nesWord requestedAddress, unsigned int& mappedAddress) override;

private:

    int switchableBankIndex;
    int fixedBankIndex;
};

#endif
