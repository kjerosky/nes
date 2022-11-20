#include "Nes.h"

#include <sstream>

Nes::Nes(Cartridge* cartridge) {
    this->cartridge = cartridge;

    ppu = new Ppu(cartridge);
    bus = new Bus(ppu, cartridge);
    cpu = new Cpu(bus);

    cycleCounter = 0;
    isContinuouslyExecuting = false;
    secondsUntilNextFrameDraw = 0;

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
    if (isContinuouslyExecuting) {
        return;
    }

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
    if (isContinuouslyExecuting) {
        return;
    }

    do {
        clockTick();
    } while (!ppu->isFrameComplete());

    // The cpu might not be done executing the current instruction,
    // so let's clock more cycles until it's complete.
    // Since the ppu will be on its prerender scanline at this point
    // (scanline = -1), a few extra clock cycles will not cause extra
    // pixels to be output to the upper-left part of the screen.
    while (!cpu->isCurrentInstructionComplete()) {
        clockTick();
    }

    ppu->acknowledgeFrameWasCompleted();
}

void Nes::toggleContinuousExecution() {
    isContinuouslyExecuting = !isContinuouslyExecuting;
    if (!isContinuouslyExecuting) {
        while (!cpu->isCurrentInstructionComplete()) {
            clockTick();
        }
    }
}

void Nes::processTimeElapsed(float secondsElapsed) {
    if (!isContinuouslyExecuting) {
        return;
    }

    if (secondsUntilNextFrameDraw <= 0) {
        secondsUntilNextFrameDraw += SECONDS_BETWEEN_FRAMES - secondsElapsed;

        while (!ppu->isFrameComplete()) {
            clockTick();
        }

        ppu->acknowledgeFrameWasCompleted();
    } else {
        secondsUntilNextFrameDraw -= secondsElapsed;
    }
}

CpuInfo Nes::getCpuInfo() {
    return cpu->getInfo();
}

nesByte* Nes::getCpuRam() {
    return bus->ram;
}

olc::Sprite* Nes::getPatternTable(int patternTableIndex, int paletteIndex) {
    return ppu->getPatternTable(patternTableIndex, paletteIndex);
}

olc::Pixel* Nes::getActivePalettesColors() {
    return ppu->getActivePalettesColors();
}

nesByte* Nes::getNameTable(int nameTableIndex) {
    return ppu->getNameTable(nameTableIndex);
}

void Nes::reset() {
    ppu->reset();
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

void Nes::nmi() {
    cpu->nmi();
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

    if (ppu->checkAndResetNmiSignal()) {
        cpu->nmi();
    }

    cycleCounter++;
}
