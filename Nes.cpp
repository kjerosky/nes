#include "Nes.h"

#include <sstream>

Nes::Nes(nesWord mainCodeRamOffset, std::stringstream& mainCodeBytesStream, nesWord irqCodeRamOffset, std::stringstream& irqCodeBytesStream) {
    bus = new Bus();
    bus->ram[0xFFFC] = mainCodeRamOffset & 0x00FF;
    bus->ram[0xFFFD] = (mainCodeRamOffset >> 8) & 0x00FF;
    bus->ram[0xFFFE] = irqCodeRamOffset & 0x00FF;
    bus->ram[0xFFFF] = (irqCodeRamOffset >> 8) & 0x00FF;

    while (!mainCodeBytesStream.eof()) {
        std::string programByte;
        mainCodeBytesStream >> programByte;
        bus->ram[mainCodeRamOffset++] = (nesByte)std::stoul(programByte, nullptr, 16);
    }

    while (!irqCodeBytesStream.eof()) {
        std::string programByte;
        irqCodeBytesStream >> programByte;
        bus->ram[irqCodeRamOffset++] = (nesByte)std::stoul(programByte, nullptr, 16);
    }

    cpu = new Cpu(bus);
    reset();
}

Nes::~Nes() {
    delete bus;
    delete cpu;
}

std::map<nesWord, std::string> Nes::disassemble(nesWord lowerAddress, nesWord upperAddress) {
    return cpu->disassemble(lowerAddress, upperAddress);
}

void Nes::executeNextInstruction() {
    do {
        clockTick();
    } while (!cpu->isCurrentInstructionComplete());
}

CpuInfo Nes::getCpuInfo() {
    return cpu->getInfo();
}

nesByte* Nes::getCpuRam() {
    return bus->ram;
}

void Nes::reset() {
    cpu->reset();
    while (!cpu->isCurrentInstructionComplete()) {
        clockTick();
    }
}

void Nes::irq() {
    cpu->irq();
    while (!cpu->isCurrentInstructionComplete()) {
        clockTick();
    }
}

void Nes::clockTick() {
    cpu->clockTick();
}
