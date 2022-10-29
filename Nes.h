#ifndef NES_H
#define NES_H

#include <sstream>
#include <map>

#include "Types.h"
#include "Cpu.h"
#include "Bus.h"

class Nes {

private:

    Cpu* cpu;
    Bus* bus;

public:

    Nes(
        nesWord mainCodeRamOffset,
        std::stringstream& mainCodeBytesStream,
        nesWord irqCodeRamOffset,
        std::stringstream& irqCodeBytesStream
    );
    ~Nes();

    std::map<nesWord, std::string> disassemble(nesWord lowerAddress, nesWord upperAddress);
    void executeNextInstruction();
    CpuInfo getCpuInfo();
    nesByte* getCpuRam();

    void reset();
    void irq();

private:

    void clockTick();
};

#endif
