#include "Cpu.h"

Cpu::Cpu(Bus* bus) {
    this->bus = bus;

    initializeOpcodeTable();

    reset();
}

Cpu::~Cpu() {
    // do nothing
}

void Cpu::initializeOpcodeTable() {
    Instruction invalidInstruction = {"???", 2, &Cpu::implied, &Cpu::invalid};
    opcodeTable = std::vector<Instruction>(256, invalidInstruction);

    //TODO OVERRIDE VALID OPCODES HERE
}

CpuDebugInfo Cpu::getDebugInfo() {
    CpuDebugInfo debugInfo;
    debugInfo.a = a;
    debugInfo.x = x;
    debugInfo.y = y;
    debugInfo.sp = sp;
    debugInfo.status = status;
    debugInfo.pc = pc;
    debugInfo.nFlag = status & N_FLAG;
    debugInfo.vFlag = status & V_FLAG;
    debugInfo.dFlag = status & D_FLAG;
    debugInfo.iFlag = status & I_FLAG;
    debugInfo.zFlag = status & Z_FLAG;
    debugInfo.cFlag = status & C_FLAG;

    return debugInfo;
}

void Cpu::reset() {
    nesWord newPcLo = bus->read(0xFFFC);
    nesWord newPcHi = bus->read(0xFFFD);
    pc = (newPcHi << 8) | newPcLo;

    a = 0;
    x = 0;
    y = 0;
    sp = 0xFF;
    status = 0;

    cyclesRemaining = 0;
}

void Cpu::clockTick() {
    if (cyclesRemaining <= 0) {
        opcode = bus->read(pc++);

        //TODO LOOKUP OPCODE

        //TODO GET ADDITIONAL CYCLES FROM ADDRESS MODE
        //TODO GET ADDITIONAL CYCLES FROM OPERATION
        //TODO ADD ADDITIONAL CYCLES TO REMAINING CYCLES
    }

    cyclesRemaining--;
}

bool Cpu::isCurrentInstructionComplete() {
    return cyclesRemaining <= 0;
}

nesByte Cpu::getStatusFlag(nesByte flag) {
    return status & flag ? 1 : 0;
}

void Cpu::setStatusFlag(nesByte flag, bool value) {
    if (value) {
        status |= flag;
    } else {
        status &= ~flag;
    }
}

int Cpu::implied() {
    return 0;
}

int Cpu::invalid() {
    return 0;
}
