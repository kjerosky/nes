#include "Cpu.h"
#include "TextUtils.h"

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

    opcodeTable[0x00] = {"BRK", 7, &Cpu::implied, &Cpu::BRK};
    opcodeTable[0x08] = {"PHP", 3, &Cpu::implied, &Cpu::PHP};
    opcodeTable[0x0A] = {"ASL", 2, &Cpu::implied, &Cpu::ASL};
    opcodeTable[0x18] = {"CLC", 2, &Cpu::implied, &Cpu::CLC};
    opcodeTable[0x28] = {"PLP", 4, &Cpu::implied, &Cpu::PLP};
    opcodeTable[0x2A] = {"ROL", 2, &Cpu::implied, &Cpu::ROL};
    opcodeTable[0x38] = {"SEC", 2, &Cpu::implied, &Cpu::SEC};
    opcodeTable[0x40] = {"RTI", 6, &Cpu::implied, &Cpu::RTI};
    opcodeTable[0x48] = {"PHA", 3, &Cpu::implied, &Cpu::PHA};
    opcodeTable[0x4A] = {"LSR", 2, &Cpu::implied, &Cpu::LSR};
    opcodeTable[0x58] = {"CLI", 2, &Cpu::implied, &Cpu::CLI};
    opcodeTable[0x60] = {"RTS", 6, &Cpu::implied, &Cpu::RTS};
    opcodeTable[0x68] = {"PLA", 4, &Cpu::implied, &Cpu::PLA};
    opcodeTable[0x6A] = {"ROR", 2, &Cpu::implied, &Cpu::ROR};
    opcodeTable[0x78] = {"SEI", 2, &Cpu::implied, &Cpu::SEI};
    opcodeTable[0x88] = {"DEY", 2, &Cpu::implied, &Cpu::DEY};
    opcodeTable[0x8A] = {"TXA", 2, &Cpu::implied, &Cpu::TXA};
    opcodeTable[0x98] = {"TYA", 2, &Cpu::implied, &Cpu::TYA};
    opcodeTable[0x9A] = {"TXS", 2, &Cpu::implied, &Cpu::TXS};
    opcodeTable[0xA8] = {"TAY", 2, &Cpu::implied, &Cpu::TAY};
    opcodeTable[0xAA] = {"TAX", 2, &Cpu::implied, &Cpu::TAX};
    opcodeTable[0xB8] = {"CLV", 2, &Cpu::implied, &Cpu::CLV};
    opcodeTable[0xBA] = {"TSX", 2, &Cpu::implied, &Cpu::TSX};
    opcodeTable[0xC8] = {"INY", 2, &Cpu::implied, &Cpu::INY};
    opcodeTable[0xCA] = {"DEX", 2, &Cpu::implied, &Cpu::DEX};
    opcodeTable[0xD8] = {"CLD", 2, &Cpu::implied, &Cpu::CLD};
    opcodeTable[0xE8] = {"INX", 2, &Cpu::implied, &Cpu::INX};
    opcodeTable[0xEA] = {"NOP", 2, &Cpu::implied, &Cpu::NOP};
    opcodeTable[0xF8] = {"SED", 2, &Cpu::implied, &Cpu::SED};

    opcodeTable[0x0D] = {"ORA", 4, &Cpu::absolute, &Cpu::ORA};
    opcodeTable[0x0E] = {"ASL", 6, &Cpu::absolute, &Cpu::ASL};
    opcodeTable[0x20] = {"JSR", 6, &Cpu::absolute, &Cpu::JSR};
    opcodeTable[0x2C] = {"BIT", 4, &Cpu::absolute, &Cpu::BIT};
    opcodeTable[0x2D] = {"AND", 4, &Cpu::absolute, &Cpu::AND};
    opcodeTable[0x2E] = {"ROL", 6, &Cpu::absolute, &Cpu::ROL};
    opcodeTable[0x4C] = {"JMP", 3, &Cpu::absolute, &Cpu::JMP};
    opcodeTable[0x4D] = {"EOR", 4, &Cpu::absolute, &Cpu::EOR};
    opcodeTable[0x4E] = {"LSR", 6, &Cpu::absolute, &Cpu::LSR};
    opcodeTable[0x6D] = {"ADC", 4, &Cpu::absolute, &Cpu::ADC};
    opcodeTable[0x6E] = {"ROR", 6, &Cpu::absolute, &Cpu::ROR};
    opcodeTable[0x8C] = {"STY", 4, &Cpu::absolute, &Cpu::STY};
    opcodeTable[0x8D] = {"STA", 4, &Cpu::absolute, &Cpu::STA};
    opcodeTable[0x8E] = {"STX", 4, &Cpu::absolute, &Cpu::STX};
    opcodeTable[0xAC] = {"LDY", 4, &Cpu::absolute, &Cpu::LDY};
    opcodeTable[0xAD] = {"LDA", 4, &Cpu::absolute, &Cpu::LDA};
    opcodeTable[0xAE] = {"LDX", 4, &Cpu::absolute, &Cpu::LDX};
    opcodeTable[0xCC] = {"CPY", 4, &Cpu::absolute, &Cpu::CPY};
    opcodeTable[0xCD] = {"CMP", 4, &Cpu::absolute, &Cpu::CMP};
    opcodeTable[0xCE] = {"DEC", 6, &Cpu::absolute, &Cpu::DEC};
    opcodeTable[0xEC] = {"CPX", 4, &Cpu::absolute, &Cpu::CPX};
    opcodeTable[0xED] = {"SBC", 4, &Cpu::absolute, &Cpu::SBC};
    opcodeTable[0xEE] = {"INC", 6, &Cpu::absolute, &Cpu::INC};

    opcodeTable[0x1D] = {"ORA", 4, &Cpu::absoluteXIndexed, &Cpu::ORA};
    opcodeTable[0x1E] = {"ASL", 7, &Cpu::absoluteXIndexed, &Cpu::ASL};
    opcodeTable[0x3D] = {"AND", 4, &Cpu::absoluteXIndexed, &Cpu::AND};
    opcodeTable[0x3E] = {"ROL", 7, &Cpu::absoluteXIndexed, &Cpu::ROL};
    opcodeTable[0x5D] = {"EOR", 4, &Cpu::absoluteXIndexed, &Cpu::EOR};
    opcodeTable[0x5E] = {"LSR", 7, &Cpu::absoluteXIndexed, &Cpu::LSR};
    opcodeTable[0x7D] = {"ADC", 4, &Cpu::absoluteXIndexed, &Cpu::ADC};
    opcodeTable[0x7E] = {"ROR", 7, &Cpu::absoluteXIndexed, &Cpu::ROR};
    opcodeTable[0x9D] = {"STA", 5, &Cpu::absoluteXIndexed, &Cpu::STA};
    opcodeTable[0xBC] = {"LDY", 4, &Cpu::absoluteXIndexed, &Cpu::LDY};
    opcodeTable[0xBD] = {"LDA", 4, &Cpu::absoluteXIndexed, &Cpu::LDA};
    opcodeTable[0xDD] = {"CMP", 4, &Cpu::absoluteXIndexed, &Cpu::CMP};
    opcodeTable[0xDE] = {"DEC", 7, &Cpu::absoluteXIndexed, &Cpu::DEC};
    opcodeTable[0xFD] = {"SBC", 4, &Cpu::absoluteXIndexed, &Cpu::SBC};
    opcodeTable[0xFE] = {"INC", 7, &Cpu::absoluteXIndexed, &Cpu::INC};

    opcodeTable[0x19] = {"ORA", 4, &Cpu::absoluteYIndexed, &Cpu::ORA};
    opcodeTable[0x39] = {"AND", 4, &Cpu::absoluteYIndexed, &Cpu::AND};
    opcodeTable[0x59] = {"EOR", 4, &Cpu::absoluteYIndexed, &Cpu::EOR};
    opcodeTable[0x79] = {"ADC", 4, &Cpu::absoluteYIndexed, &Cpu::ADC};
    opcodeTable[0x99] = {"STA", 5, &Cpu::absoluteYIndexed, &Cpu::STA};
    opcodeTable[0xB9] = {"LDA", 4, &Cpu::absoluteYIndexed, &Cpu::LDA};
    opcodeTable[0xBE] = {"LDX", 4, &Cpu::absoluteYIndexed, &Cpu::LDX};
    opcodeTable[0xD9] = {"CMP", 4, &Cpu::absoluteYIndexed, &Cpu::CMP};
    opcodeTable[0xF9] = {"SBC", 4, &Cpu::absoluteYIndexed, &Cpu::SBC};

    opcodeTable[0x09] = {"ORA", 2, &Cpu::immediate, &Cpu::ORA};
    opcodeTable[0x29] = {"AND", 2, &Cpu::immediate, &Cpu::AND};
    opcodeTable[0x49] = {"EOR", 2, &Cpu::immediate, &Cpu::EOR};
    opcodeTable[0x69] = {"ADC", 2, &Cpu::immediate, &Cpu::ADC};
    opcodeTable[0xA0] = {"LDY", 2, &Cpu::immediate, &Cpu::LDY};
    opcodeTable[0xA2] = {"LDX", 2, &Cpu::immediate, &Cpu::LDX};
    opcodeTable[0xA9] = {"LDA", 2, &Cpu::immediate, &Cpu::LDA};
    opcodeTable[0xC0] = {"CPY", 2, &Cpu::immediate, &Cpu::CPY};
    opcodeTable[0xC9] = {"CMP", 2, &Cpu::immediate, &Cpu::CMP};
    opcodeTable[0xE0] = {"CPX", 2, &Cpu::immediate, &Cpu::CPX};
    opcodeTable[0xE9] = {"SBC", 2, &Cpu::immediate, &Cpu::SBC};

    opcodeTable[0x6C] = {"JMP", 5, &Cpu::indirect, &Cpu::JMP};

    opcodeTable[0x01] = {"ORA", 6, &Cpu::xIndexedZeropageIndirect, &Cpu::ORA};
    opcodeTable[0x21] = {"AND", 6, &Cpu::xIndexedZeropageIndirect, &Cpu::AND};
    opcodeTable[0x41] = {"EOR", 6, &Cpu::xIndexedZeropageIndirect, &Cpu::EOR};
    opcodeTable[0x61] = {"ADC", 6, &Cpu::xIndexedZeropageIndirect, &Cpu::ADC};
    opcodeTable[0x81] = {"STA", 6, &Cpu::xIndexedZeropageIndirect, &Cpu::STA};
    opcodeTable[0xA1] = {"LDA", 6, &Cpu::xIndexedZeropageIndirect, &Cpu::LDA};
    opcodeTable[0xC1] = {"CMP", 6, &Cpu::xIndexedZeropageIndirect, &Cpu::CMP};
    opcodeTable[0xE1] = {"SBC", 6, &Cpu::xIndexedZeropageIndirect, &Cpu::SBC};

    opcodeTable[0x11] = {"ORA", 5, &Cpu::zeropageIndirectYIndexed, &Cpu::ORA};
    opcodeTable[0x31] = {"AND", 5, &Cpu::zeropageIndirectYIndexed, &Cpu::AND};
    opcodeTable[0x51] = {"EOR", 5, &Cpu::zeropageIndirectYIndexed, &Cpu::EOR};
    opcodeTable[0x71] = {"ADC", 5, &Cpu::zeropageIndirectYIndexed, &Cpu::ADC};
    opcodeTable[0x91] = {"STA", 6, &Cpu::zeropageIndirectYIndexed, &Cpu::STA};
    opcodeTable[0xB1] = {"LDA", 5, &Cpu::zeropageIndirectYIndexed, &Cpu::LDA};
    opcodeTable[0xD1] = {"CMP", 5, &Cpu::zeropageIndirectYIndexed, &Cpu::CMP};
    opcodeTable[0xF1] = {"SBC", 5, &Cpu::zeropageIndirectYIndexed, &Cpu::SBC};

    opcodeTable[0x10] = {"BPL", 2, &Cpu::relative, &Cpu::BPL};
    opcodeTable[0x30] = {"BMI", 2, &Cpu::relative, &Cpu::BMI};
    opcodeTable[0x50] = {"BVC", 2, &Cpu::relative, &Cpu::BVC};
    opcodeTable[0x70] = {"BVS", 2, &Cpu::relative, &Cpu::BVS};
    opcodeTable[0x90] = {"BCC", 2, &Cpu::relative, &Cpu::BCC};
    opcodeTable[0xB0] = {"BCS", 2, &Cpu::relative, &Cpu::BCS};
    opcodeTable[0xD0] = {"BNE", 2, &Cpu::relative, &Cpu::BNE};
    opcodeTable[0xF0] = {"BEQ", 2, &Cpu::relative, &Cpu::BEQ};

    opcodeTable[0x05] = {"ORA", 3, &Cpu::zeropage, &Cpu::ORA};
    opcodeTable[0x06] = {"ASL", 5, &Cpu::zeropage, &Cpu::ASL};
    opcodeTable[0x24] = {"BIT", 3, &Cpu::zeropage, &Cpu::BIT};
    opcodeTable[0x25] = {"AND", 3, &Cpu::zeropage, &Cpu::AND};
    opcodeTable[0x26] = {"ROL", 5, &Cpu::zeropage, &Cpu::ROL};
    opcodeTable[0x45] = {"EOR", 3, &Cpu::zeropage, &Cpu::EOR};
    opcodeTable[0x46] = {"LSR", 5, &Cpu::zeropage, &Cpu::LSR};
    opcodeTable[0x65] = {"ADC", 3, &Cpu::zeropage, &Cpu::ADC};
    opcodeTable[0x66] = {"ROR", 5, &Cpu::zeropage, &Cpu::ROR};
    opcodeTable[0x84] = {"STY", 3, &Cpu::zeropage, &Cpu::STY};
    opcodeTable[0x85] = {"STA", 3, &Cpu::zeropage, &Cpu::STA};
    opcodeTable[0x86] = {"STX", 3, &Cpu::zeropage, &Cpu::STX};
    opcodeTable[0xA4] = {"LDY", 3, &Cpu::zeropage, &Cpu::LDY};
    opcodeTable[0xA5] = {"LDA", 3, &Cpu::zeropage, &Cpu::LDA};
    opcodeTable[0xA6] = {"LDX", 3, &Cpu::zeropage, &Cpu::LDX};
    opcodeTable[0xC4] = {"CPY", 3, &Cpu::zeropage, &Cpu::CPY};
    opcodeTable[0xC5] = {"CMP", 3, &Cpu::zeropage, &Cpu::CMP};
    opcodeTable[0xC6] = {"DEC", 5, &Cpu::zeropage, &Cpu::DEC};
    opcodeTable[0xE4] = {"CPX", 3, &Cpu::zeropage, &Cpu::CPX};
    opcodeTable[0xE5] = {"SBC", 3, &Cpu::zeropage, &Cpu::SBC};
    opcodeTable[0xE6] = {"INC", 5, &Cpu::zeropage, &Cpu::INC};

    opcodeTable[0x15] = {"ORA", 4, &Cpu::zeropageXIndexed, &Cpu::ORA};
    opcodeTable[0x16] = {"ASL", 6, &Cpu::zeropageXIndexed, &Cpu::ASL};
    opcodeTable[0x35] = {"AND", 4, &Cpu::zeropageXIndexed, &Cpu::AND};
    opcodeTable[0x36] = {"ROL", 6, &Cpu::zeropageXIndexed, &Cpu::ROL};
    opcodeTable[0x55] = {"EOR", 4, &Cpu::zeropageXIndexed, &Cpu::EOR};
    opcodeTable[0x56] = {"LSR", 6, &Cpu::zeropageXIndexed, &Cpu::LSR};
    opcodeTable[0x75] = {"ADC", 4, &Cpu::zeropageXIndexed, &Cpu::ADC};
    opcodeTable[0x76] = {"ROR", 6, &Cpu::zeropageXIndexed, &Cpu::ROR};
    opcodeTable[0x94] = {"STY", 4, &Cpu::zeropageXIndexed, &Cpu::STY};
    opcodeTable[0x95] = {"STA", 4, &Cpu::zeropageXIndexed, &Cpu::STA};
    opcodeTable[0xB4] = {"LDY", 4, &Cpu::zeropageXIndexed, &Cpu::LDY};
    opcodeTable[0xB5] = {"LDA", 4, &Cpu::zeropageXIndexed, &Cpu::LDA};
    opcodeTable[0xD5] = {"CMP", 4, &Cpu::zeropageXIndexed, &Cpu::CMP};
    opcodeTable[0xD6] = {"DEC", 6, &Cpu::zeropageXIndexed, &Cpu::DEC};
    opcodeTable[0xF5] = {"SBC", 4, &Cpu::zeropageXIndexed, &Cpu::SBC};
    opcodeTable[0xF6] = {"INC", 6, &Cpu::zeropageXIndexed, &Cpu::INC};

    opcodeTable[0x96] = {"STX", 4, &Cpu::zeropageYIndexed, &Cpu::STX};
    opcodeTable[0xB6] = {"LDX", 4, &Cpu::zeropageYIndexed, &Cpu::LDX};
}

CpuInfo Cpu::getInfo() {
    CpuInfo cpuInfo;
    cpuInfo.a = a;
    cpuInfo.x = x;
    cpuInfo.y = y;
    cpuInfo.sp = sp;
    cpuInfo.status = status;
    cpuInfo.pc = pc;
    cpuInfo.nFlag = status & N_FLAG;
    cpuInfo.vFlag = status & V_FLAG;
    cpuInfo.dFlag = status & D_FLAG;
    cpuInfo.iFlag = status & I_FLAG;
    cpuInfo.zFlag = status & Z_FLAG;
    cpuInfo.cFlag = status & C_FLAG;

    return cpuInfo;
}

void Cpu::reset() {
    nesWord newPcLo = bus->cpuRead(0xFFFC);
    nesWord newPcHi = bus->cpuRead(0xFFFD);
    pc = (newPcHi << 8) | newPcLo;

    a = 0;
    x = 0;
    y = 0;
    sp = 0xFF;
    status = 0;

    cyclesRemaining = 8;
}

void Cpu::irq() {
    if (getStatusFlag(I_FLAG)) {
        return;
    }

    bus->cpuWrite(0x0100 + sp, (pc >> 8) & 0x00FF);
    sp--;
    bus->cpuWrite(0x0100 + sp, pc & 0x00FF);
    sp--;
    bus->cpuWrite(0x0100 + sp, status | U_FLAG);
    sp--;

    setStatusFlag(I_FLAG, true);

    nesWord newPcLo = bus->cpuRead(0xFFFE);
    nesWord newPcHi = bus->cpuRead(0xFFFF);
    pc = (newPcHi << 8) | newPcLo;

    cyclesRemaining = 7;
}

void Cpu::clockTick() {
    if (cyclesRemaining <= 0) {
        nesByte opcodeByte = bus->cpuRead(pc++);

        opcode = opcodeTable[opcodeByte];
        cyclesRemaining = opcode.cycles;
        addressMode = opcode.addressMode;
        bool addressModeRequiresAdditionalCycles = (this->*opcode.addressMode)();
        bool operationExecutionRequiresAdditionalCycles = (this->*opcode.execute)();
        if (addressModeRequiresAdditionalCycles && operationExecutionRequiresAdditionalCycles) {
            cyclesRemaining++;
        }
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

void Cpu::fetch() {
    if (addressMode != &Cpu::implied) {
        fetchedByte = bus->cpuRead(absoluteAddress);
    }
}



// address modes

bool Cpu::implied() {
    fetchedByte = a;
    return false;
}

bool Cpu::absolute() {
    nesWord loAddressByte = bus->cpuRead(pc++);
    nesWord hiAddressByte = bus->cpuRead(pc++);
    absoluteAddress = (hiAddressByte << 8) | loAddressByte;
    return false;
}

bool Cpu::absoluteXIndexed() {
    nesWord loAddressByte = bus->cpuRead(pc++);
    nesWord hiAddressByte = bus->cpuRead(pc++);
    absoluteAddress = ((hiAddressByte << 8) | loAddressByte) + x;

    return (absoluteAddress & 0xFF00) != (hiAddressByte << 8);
}

bool Cpu::absoluteYIndexed() {
    nesWord loAddressByte = bus->cpuRead(pc++);
    nesWord hiAddressByte = bus->cpuRead(pc++);
    absoluteAddress = ((hiAddressByte << 8) | loAddressByte) + y;

    return (absoluteAddress & 0xFF00) != (hiAddressByte << 8);
}

bool Cpu::immediate() {
    absoluteAddress = pc++;
    return false;
}

bool Cpu::indirect() {
    nesWord loPointerAddress = bus->cpuRead(pc++);
    nesWord hiPointerAddress = bus->cpuRead(pc++);
    nesWord pointerAddress = (hiPointerAddress << 8) | loPointerAddress;

    nesWord newAbsoluteAddressLo = bus->cpuRead(pointerAddress);
    nesWord newAbsoluteAddressHi;
    if (loPointerAddress == 0xFF) {
        // simulate indirect jump page boundary bug (https://www.nesdev.org/wiki/Errata#CPU)
        newAbsoluteAddressHi = bus->cpuRead(pointerAddress & 0xFF00);
    } else {
        newAbsoluteAddressHi = bus->cpuRead(pointerAddress + 1);
    }

    absoluteAddress = (newAbsoluteAddressHi << 8) | newAbsoluteAddressLo;

    return false;
}

bool Cpu::xIndexedZeropageIndirect() {
    nesWord zeroPageAddress = bus->cpuRead(pc++) + x;

    nesWord loPointerAddress = bus->cpuRead(zeroPageAddress & 0x00FF);
    nesWord hiPointerAddress = bus->cpuRead((zeroPageAddress + 1) & 0x00FF);
    absoluteAddress = (hiPointerAddress << 8) | loPointerAddress;

    return false;
}

bool Cpu::zeropageIndirectYIndexed() {
    nesWord zeroPageAddress = bus->cpuRead(pc++);

    nesWord loPointerAddress = bus->cpuRead(zeroPageAddress & 0x00FF);
    nesWord hiPointerAddress = bus->cpuRead((zeroPageAddress + 1) & 0x00FF);
    absoluteAddress = ((hiPointerAddress << 8) | loPointerAddress) + y;

    return (absoluteAddress & 0xFF00) != (hiPointerAddress << 8);
}

bool Cpu::relative() {
    relativeAddress = bus->cpuRead(pc++);
    if (relativeAddress & 0x0080) {
        relativeAddress |= 0xFF00;
    }

    return false;
}

bool Cpu::zeropage() {
    absoluteAddress = bus->cpuRead(pc++);

    return false;
}

bool Cpu::zeropageXIndexed() {
    absoluteAddress = (bus->cpuRead(pc++) + x) & 0xFF;

    return false;
}

bool Cpu::zeropageYIndexed() {
    absoluteAddress = (bus->cpuRead(pc++) + y) & 0xFF;

    return false;
}



// operations

bool Cpu::invalid() {
    // do nothing for invalid operations
    return false;
}

bool Cpu::BRK() {
    pc++;
    bus->cpuWrite(0x0100 + sp, (pc >> 8) & 0x00FF);
    sp--;
    bus->cpuWrite(0x0100 + sp, pc & 0x00FF);
    sp--;
    bus->cpuWrite(0x0100 + sp, status | U_FLAG | B_FLAG);
    sp--;

    setStatusFlag(I_FLAG, true);

    nesWord newPcLo = bus->cpuRead(0xFFFE);
    nesWord newPcHi = bus->cpuRead(0xFFFF);
    pc = (newPcHi << 8) | newPcLo;

    return false;
}

bool Cpu::RTI() {
    sp++;
    status = bus->cpuRead(0x0100 + sp);
    status &= ~U_FLAG;
    status &= ~B_FLAG;

    sp++;
    nesWord newPcLo = bus->cpuRead(0x0100 + sp);
    sp++;
    nesWord newPcHi = bus->cpuRead(0x0100 + sp);
    pc = (newPcHi << 8) | newPcLo;

    return false;
}

bool Cpu::RTS() {
    sp++;
    nesWord newPcLow = bus->cpuRead(0x0100 + sp);
    sp++;
    nesWord newPcHigh = bus->cpuRead(0x0100 + sp);

    pc = (newPcHigh << 8) | newPcLow;
    pc++;

    return false;
}

bool Cpu::PHP() {
    bus->cpuWrite(0x0100 + sp, status | U_FLAG | B_FLAG);
    sp--;
    return false;
}

bool Cpu::CLC() {
    setStatusFlag(C_FLAG, 0);
    return false;
}

bool Cpu::PLP() {
    sp++;
    status = bus->cpuRead(0x0100 + sp) & (~U_FLAG) & (~B_FLAG);
    return false;
}

bool Cpu::SEC() {
    setStatusFlag(C_FLAG, 1);
    return false;
}

bool Cpu::PHA() {
    bus->cpuWrite(0x0100 + sp, a);
    sp--;
    return false;
}

bool Cpu::CLI() {
    setStatusFlag(I_FLAG, 0);
    return false;
}

bool Cpu::PLA() {
    sp++;
    a = bus->cpuRead(0x0100 + sp);
    setStatusFlag(N_FLAG, a & 0x80);
    setStatusFlag(Z_FLAG, a == 0x00);
    return false;
}

bool Cpu::SEI() {
    setStatusFlag(I_FLAG, 1);
    return false;
}

bool Cpu::DEY() {
    y--;
    setStatusFlag(N_FLAG, y & 0x80);
    setStatusFlag(Z_FLAG, y == 0x00);
    return false;
}

bool Cpu::TYA() {
    a = y;
    setStatusFlag(N_FLAG, a & 0x80);
    setStatusFlag(Z_FLAG, a == 0x00);
    return false;
}

bool Cpu::TAY() {
    y = a;
    setStatusFlag(N_FLAG, y & 0x80);
    setStatusFlag(Z_FLAG, y == 0x00);
    return false;
}

bool Cpu::CLV() {
    setStatusFlag(V_FLAG, 0);
    return false;
}

bool Cpu::INY() {
    y++;
    setStatusFlag(N_FLAG, y & 0x80);
    setStatusFlag(Z_FLAG, y == 0x00);
    return false;
}

bool Cpu::CLD() {
    setStatusFlag(D_FLAG, 0);
    return false;
}

bool Cpu::INX() {
    x++;
    setStatusFlag(N_FLAG, x & 0x80);
    setStatusFlag(Z_FLAG, x == 0x00);
    return false;
}

bool Cpu::SED() {
    setStatusFlag(D_FLAG, 1);
    return false;
}

bool Cpu::TXA() {
    a = x;
    setStatusFlag(N_FLAG, a & 0x80);
    setStatusFlag(Z_FLAG, a == 0x00);
    return false;
}

bool Cpu::TXS() {
    sp = x;
    return false;
}

bool Cpu::TAX() {
    x = a;
    setStatusFlag(N_FLAG, x & 0x80);
    setStatusFlag(Z_FLAG, x == 0x00);
    return false;
}

bool Cpu::TSX() {
    x = sp;
    setStatusFlag(N_FLAG, x & 0x80);
    setStatusFlag(Z_FLAG, x == 0x00);
    return false;
}

bool Cpu::DEX() {
    x--;
    setStatusFlag(N_FLAG, x & 0x80);
    setStatusFlag(Z_FLAG, x == 0x00);
    return false;
}

bool Cpu::NOP() {
    // literally do nothing
    return false;
}

bool Cpu::JSR() {
    pc--;
    nesByte savedPcLow = pc & 0x00FF;
    nesByte savedPcHigh = (pc >> 8) & 0x00FF;

    bus->cpuWrite(0x0100 + sp, savedPcHigh);
    sp--;
    bus->cpuWrite(0x0100 + sp, savedPcLow);
    sp--;

    pc = absoluteAddress;

    return false;
}

bool Cpu::BIT() {
    fetch();
    setStatusFlag(N_FLAG, fetchedByte & (1 << 7));
    setStatusFlag(Z_FLAG, (a & fetchedByte) == 0x00);
    setStatusFlag(V_FLAG, fetchedByte & (1 << 6));
    return false;
}

bool Cpu::JMP() {
    pc = absoluteAddress;

    return false;
}

bool Cpu::STY() {
    bus->cpuWrite(absoluteAddress, y);
    return false;
}

bool Cpu::LDY() {
    fetch();
    y = fetchedByte;
    setStatusFlag(N_FLAG, y & 0x80);
    setStatusFlag(Z_FLAG, y == 0x00);
    return true;
}

bool Cpu::CPY() {
    fetch();
    nesByte temp = y - fetchedByte;
    setStatusFlag(C_FLAG, y >= fetchedByte);
    setStatusFlag(Z_FLAG, temp == 0x00);
    setStatusFlag(N_FLAG, temp & 0x80);
    return false;
}

bool Cpu::CPX() {
    fetch();
    nesByte temp = x - fetchedByte;
    setStatusFlag(C_FLAG, x >= fetchedByte);
    setStatusFlag(Z_FLAG, temp == 0x00);
    setStatusFlag(N_FLAG, temp & 0x80);
    return false;
}

bool Cpu::ORA() {
    fetch();
    a |= fetchedByte;
    setStatusFlag(N_FLAG, a & 0x80);
    setStatusFlag(Z_FLAG, a == 0x00);
    return true;
}

bool Cpu::AND() {
    fetch();
    a &= fetchedByte;
    setStatusFlag(N_FLAG, a & 0x80);
    setStatusFlag(Z_FLAG, a == 0x00);
    return true;
}

bool Cpu::EOR() {
    fetch();
    a ^= fetchedByte;
    setStatusFlag(N_FLAG, a & 0x80);
    setStatusFlag(Z_FLAG, a == 0x00);
    return true;
}

bool Cpu::ADC() {
    fetch();
    nesWord result = a + fetchedByte + getStatusFlag(C_FLAG);
    nesByte byteResult = result;
    bool signResult = byteResult & 0x80;
    bool signOperand1 = a & 0x80;
    bool signOperand2 = fetchedByte & 0x80;

    setStatusFlag(N_FLAG, signResult);
    setStatusFlag(V_FLAG, signOperand1 == signOperand2 && signOperand1 != signResult);
    setStatusFlag(Z_FLAG, byteResult == 0x00);
    setStatusFlag(C_FLAG, result & 0xFF00);

    a = byteResult;

    return true;
}

bool Cpu::STA() {
    bus->cpuWrite(absoluteAddress, a);
    return false;
}

bool Cpu::LDA() {
    fetch();
    a = fetchedByte;
    setStatusFlag(N_FLAG, a & 0x80);
    setStatusFlag(Z_FLAG, a == 0x00);
    return true;
}

bool Cpu::CMP() {
    fetch();
    nesByte temp = a - fetchedByte;
    setStatusFlag(N_FLAG, temp & 0x80);
    setStatusFlag(Z_FLAG, temp == 0x00);
    setStatusFlag(C_FLAG, a >= fetchedByte);
    return true;
}

bool Cpu::SBC() {
    fetch();
    nesByte onesComplementFetchedByte = ~fetchedByte;
    nesWord result = a + onesComplementFetchedByte + getStatusFlag(C_FLAG);
    nesByte byteResult = result;
    bool signResult = byteResult & 0x80;
    bool signOperand1 = a & 0x80;
    bool signOperand2 = onesComplementFetchedByte & 0x80;

    setStatusFlag(N_FLAG, signResult);
    setStatusFlag(V_FLAG, signOperand1 == signOperand2 && signOperand1 != signResult);
    setStatusFlag(Z_FLAG, byteResult == 0x00);
    setStatusFlag(C_FLAG, result & 0xFF00);

    a = byteResult;

    return true;
}

bool Cpu::ASL() {
    fetch();
    nesByte temp = fetchedByte << 1;
    setStatusFlag(N_FLAG, temp & 0x80);
    setStatusFlag(Z_FLAG, temp == 0x00);
    setStatusFlag(C_FLAG, fetchedByte & 0x80);

    if (opcode.addressMode == &Cpu::implied) {
        a = temp;
    } else {
        bus->cpuWrite(absoluteAddress, temp);
    }

    return false;
}

bool Cpu::ROL() {
    fetch();
    nesByte temp = (fetchedByte << 1) | getStatusFlag(C_FLAG);
    setStatusFlag(N_FLAG, temp & 0x80);
    setStatusFlag(Z_FLAG, temp == 0x00);
    setStatusFlag(C_FLAG, fetchedByte & 0x80);

    if (opcode.addressMode == &Cpu::implied) {
        a = temp;
    } else {
        bus->cpuWrite(absoluteAddress, temp);
    }

    return false;
}

bool Cpu::LSR() {
    fetch();
    nesByte temp = fetchedByte >> 1;
    setStatusFlag(N_FLAG, 0);
    setStatusFlag(Z_FLAG, temp == 0x00);
    setStatusFlag(C_FLAG, fetchedByte & 0x01);

    if (opcode.addressMode == &Cpu::implied) {
        a = temp;
    } else {
        bus->cpuWrite(absoluteAddress, temp);
    }

    return false;
}

bool Cpu::ROR() {
    fetch();
    nesByte temp = (getStatusFlag(C_FLAG) << 7) | (fetchedByte >> 1);
    setStatusFlag(N_FLAG, temp & 0x80);
    setStatusFlag(Z_FLAG, temp == 0x00);
    setStatusFlag(C_FLAG, fetchedByte & 0x01);

    if (opcode.addressMode == &Cpu::implied) {
        a = temp;
    } else {
        bus->cpuWrite(absoluteAddress, temp);
    }

    return false;
}

bool Cpu::STX() {
    bus->cpuWrite(absoluteAddress, x);
    return false;
}

bool Cpu::LDX() {
    fetch();
    x = fetchedByte;
    setStatusFlag(N_FLAG, x & 0x80);
    setStatusFlag(Z_FLAG, x == 0x00);
    return true;
}

bool Cpu::DEC() {
    fetch();
    fetchedByte--;
    bus->cpuWrite(absoluteAddress, fetchedByte);
    setStatusFlag(N_FLAG, fetchedByte & 0x80);
    setStatusFlag(Z_FLAG, fetchedByte == 0x00);
    return false;
}

bool Cpu::INC() {
    fetch();
    fetchedByte++;
    bus->cpuWrite(absoluteAddress, fetchedByte);
    setStatusFlag(N_FLAG, fetchedByte & 0x80);
    setStatusFlag(Z_FLAG, fetchedByte == 0x00);
    return false;
}

bool Cpu::BPL() {
    if (!getStatusFlag(N_FLAG)) {
        cyclesRemaining++;

        nesWord nextAddress = pc + relativeAddress;
        if ((nextAddress & 0xFF00) != (pc & 0xFF00)) {
            cyclesRemaining++;
        }

        pc = nextAddress;
    }

    return false;
}
bool Cpu::BMI() {
    if (getStatusFlag(N_FLAG)) {
        cyclesRemaining++;

        nesWord nextAddress = pc + relativeAddress;
        if ((nextAddress & 0xFF00) != (pc & 0xFF00)) {
            cyclesRemaining++;
        }

        pc = nextAddress;
    }

    return false;
}
bool Cpu::BVC() {
    if (!getStatusFlag(V_FLAG)) {
        cyclesRemaining++;

        nesWord nextAddress = pc + relativeAddress;
        if ((nextAddress & 0xFF00) != (pc & 0xFF00)) {
            cyclesRemaining++;
        }

        pc = nextAddress;
    }

    return false;
}
bool Cpu::BVS() {
    if (getStatusFlag(V_FLAG)) {
        cyclesRemaining++;

        nesWord nextAddress = pc + relativeAddress;
        if ((nextAddress & 0xFF00) != (pc & 0xFF00)) {
            cyclesRemaining++;
        }

        pc = nextAddress;
    }

    return false;
}
bool Cpu::BCC() {
    if (!getStatusFlag(C_FLAG)) {
        cyclesRemaining++;

        nesWord nextAddress = pc + relativeAddress;
        if ((nextAddress & 0xFF00) != (pc & 0xFF00)) {
            cyclesRemaining++;
        }

        pc = nextAddress;
    }

    return false;
}
bool Cpu::BCS() {
    if (getStatusFlag(C_FLAG)) {
        cyclesRemaining++;

        nesWord nextAddress = pc + relativeAddress;
        if ((nextAddress & 0xFF00) != (pc & 0xFF00)) {
            cyclesRemaining++;
        }

        pc = nextAddress;
    }

    return false;
}
bool Cpu::BNE() {
    if (!getStatusFlag(Z_FLAG)) {
        cyclesRemaining++;

        nesWord nextAddress = pc + relativeAddress;
        if ((nextAddress & 0xFF00) != (pc & 0xFF00)) {
            cyclesRemaining++;
        }

        pc = nextAddress;
    }

    return false;
}
bool Cpu::BEQ() {
    if (getStatusFlag(Z_FLAG)) {
        cyclesRemaining++;

        nesWord nextAddress = pc + relativeAddress;
        if ((nextAddress & 0xFF00) != (pc & 0xFF00)) {
            cyclesRemaining++;
        }

        pc = nextAddress;
    }

    return false;
}

std::map<nesWord, std::string> Cpu::disassemble(nesWord lowerAddress, nesWord upperAddress) {
    std::map<nesWord, std::string> disassembly;
    unsigned int currentAddress = lowerAddress;
    while (currentAddress <= upperAddress) {
        nesWord instructionAddress = currentAddress;
        nesByte opcodeByte = bus->cpuRead(currentAddress++, true);
        Instruction instruction = opcodeTable[opcodeByte];
        std::string line = "$" + hex(instructionAddress, 4) + ": " + instruction.name + " ";

        nesWord byte1;
        nesWord byte2;
        if (instruction.addressMode == &Cpu::implied) {
            // Since an RTI after a BRK will return to the second byte after the BRK,
            // we'll skip the next instruction for disassembly, like the BRK does.
            if (instruction.execute == &Cpu::BRK) {
                currentAddress++;
            }
        } else if (instruction.addressMode == &Cpu::absolute) {
            byte1 = bus->cpuRead(currentAddress++, true);
            byte2 = bus->cpuRead(currentAddress++, true);
            line += "$" + hex((byte2 << 8) | byte1, 4);
        } else if (instruction.addressMode == &Cpu::absoluteXIndexed) {
            byte1 = bus->cpuRead(currentAddress++, true);
            byte2 = bus->cpuRead(currentAddress++, true);
            line += "$" + hex((byte2 << 8) | byte1, 4) + ",X";
        } else if (instruction.addressMode == &Cpu::absoluteYIndexed) {
            byte1 = bus->cpuRead(currentAddress++, true);
            byte2 = bus->cpuRead(currentAddress++, true);
            line += "$" + hex((byte2 << 8) | byte1, 4) + ",Y";
        } else if (instruction.addressMode == &Cpu::immediate) {
            byte1 = bus->cpuRead(currentAddress++, true);
            line += "#$" + hex(byte1, 2);
        } else if (instruction.addressMode == &Cpu::indirect) {
            byte1 = bus->cpuRead(currentAddress++, true);
            byte2 = bus->cpuRead(currentAddress++, true);
            line += "($" + hex((byte2 << 8) | byte1, 4) + ")";
        } else if (instruction.addressMode == &Cpu::xIndexedZeropageIndirect) {
            byte1 = bus->cpuRead(currentAddress++, true);
            line += "($" + hex(byte1, 2) + ",X)";
        } else if (instruction.addressMode == &Cpu::zeropageIndirectYIndexed) {
            byte1 = bus->cpuRead(currentAddress++, true);
            line += "($" + hex(byte1, 2) + "),Y";
        } else if (instruction.addressMode == &Cpu::relative) {
            byte1 = bus->cpuRead(currentAddress++, true);
            if (byte1 & 0x0080) {
                byte1 |= 0xFF00;
            }

            line += "$" + hex(byte1, 2) + "   [$" + hex(currentAddress + byte1, 4) + "]";
        } else if (instruction.addressMode == &Cpu::zeropage) {
            byte1 = bus->cpuRead(currentAddress++, true);
            line += "$" + hex(byte1, 2);
        } else if (instruction.addressMode == &Cpu::zeropageXIndexed) {
            byte1 = bus->cpuRead(currentAddress++, true);
            line += "$" + hex(byte1, 2) + ",X";
        } else if (instruction.addressMode == &Cpu::zeropageYIndexed) {
            byte1 = bus->cpuRead(currentAddress++, true);
            line += "$" + hex(byte1, 2) + ",Y";
        }

        disassembly[instructionAddress] = line;
    }

    return disassembly;
}
