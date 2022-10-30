#include "Nes.h"

#include <sstream>

Nes::Nes(nesWord mainCodeRamOffset, std::stringstream& mainCodeBytesStream, nesWord irqCodeRamOffset, std::stringstream& irqCodeBytesStream) {
    ppu = new Ppu();

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

    cycleCounter = 0;
    reset();
}

Nes::~Nes() {
    delete bus;
    delete ppu;
    delete cpu;
}

olc::Sprite* Nes::getScreen() {
    return ppu->getScreen();
}

std::map<nesWord, std::string> Nes::disassemble(nesWord lowerAddress, nesWord upperAddress) {
    return cpu->disassemble(lowerAddress, upperAddress);
}

void Nes::executeNextInstruction() {
    // Since the cpu clock runs slower than the system clock, the cpu
    // might report as having already completed its instruction for
    // extra cycles, so we'll drain those out here before attempting
    // to execute the next actual instruction.
    while (cpu->isCurrentInstructionComplete()) {
        clockTick();
    }

    do {
        clockTick();
    } while (!cpu->isCurrentInstructionComplete());
}

void Nes::displayNextFrame() {
    do {
        clockTick();
    } while (!ppu->isFrameComplete());

    // The cpu might not be done executing the current instruction,
    // so let's clock more cycles until it's complete.
    // This may end up drawing a few extra pixels in the upper-left
    // part of the screen, but it's better to have the cpu in a
    // stable state.
    while (!cpu->isCurrentInstructionComplete()) {
        clockTick();
    }

    ppu->acknowledgeFrameWasCompleted();
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
    ppu->clockTick();

    if (cycleCounter % 3 == 0) {
        cycleCounter = 0;
        cpu->clockTick();
    }

    cycleCounter++;
}
