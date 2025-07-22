#include "Nes.h"

#include <sstream>

Nes::Nes(Cartridge* cartridge, SDL_Texture* screen_texture, const SDL_PixelFormatDetails* pixel_format_details) {
    this->cartridge = cartridge;

    ppu = new Ppu(cartridge, screen_texture, pixel_format_details);
    apu = new Apu();
    bus = new Bus(ppu, apu, cartridge);
    cpu = new Cpu(bus);

    cycleCounter = 0;
    isContinuouslyExecuting = true;
    secondsUntilNextFrameDraw = 0;

    reset();
}

Nes::~Nes() {
    delete bus;
    delete ppu;
    delete cpu;
    delete apu;
}

std::map<Uint16, std::string> Nes::disassemble(Uint16 lowerAddress, Uint16 upperAddress) {
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

Uint8* Nes::getCpuRam() {
    return bus->ram;
}

Uint8* Nes::getNameTable(int nameTableIndex) {
    return ppu->getNameTable(nameTableIndex);
}

void Nes::reset() {
    audioSample = 0;
    audioTime = 0;

    cartridge->reset();
    ppu->reset();
    cpu->reset();
    apu->reset();
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

void Nes::setAudioSampleRate(int sampleRate) {
    audioSampleTime = 1.0 / sampleRate;
    nesClockCycleTime = 1.0 / 5369318; // ppu clock cycle time
}

float Nes::getAudioSample() {
    return audioSample;
}

bool Nes::clockForAudioSample() {
    bool audioSampleIsReady = false;

    clockTick();

    audioTime += nesClockCycleTime;
    if (audioTime >= audioSampleTime) {
        audioTime -= audioSampleTime;
        audioSample = apu->getAudioSampleOutput();
        audioSampleIsReady = true;
    }

    return audioSampleIsReady;
}

void Nes::clockTick() {
    ppu->clockTick();
    apu->clockTick();

    if (cycleCounter % 3 == 0) {
        cycleCounter = 0;

        if (!bus->checkDmaProgress()) {
            cpu->clockTick();
        }
    }

    if (ppu->checkAndResetNmiSignal()) {
        cpu->nmi();
    }

    cycleCounter++;
}

void Nes::updateControllerStates(Uint8 controller1State, Uint8 controller2State) {
    bus->updateControllerStates(controller1State, controller2State);
}

Uint8* Nes::getOamBytes() {
    return ppu->getOamBytes();
}
