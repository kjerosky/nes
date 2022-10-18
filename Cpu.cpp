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

    opcodeTable[0x01] = {"ORA", 6, &Cpu::indexedIndirect, &Cpu::ORA};
    opcodeTable[0x21] = {"AND", 6, &Cpu::indexedIndirect, &Cpu::AND};
    opcodeTable[0x41] = {"EOR", 6, &Cpu::indexedIndirect, &Cpu::EOR};
    opcodeTable[0x61] = {"ADC", 6, &Cpu::indexedIndirect, &Cpu::ADC};
    opcodeTable[0x81] = {"STA", 6, &Cpu::indexedIndirect, &Cpu::STA};
    opcodeTable[0xA1] = {"LDA", 6, &Cpu::indexedIndirect, &Cpu::LDA};
    opcodeTable[0xC1] = {"CMP", 6, &Cpu::indexedIndirect, &Cpu::CMP};
    opcodeTable[0xE1] = {"SBC", 6, &Cpu::indexedIndirect, &Cpu::SBC};

    opcodeTable[0x11] = {"ORA", 5, &Cpu::indirectIndexed, &Cpu::ORA};
    opcodeTable[0x31] = {"AND", 5, &Cpu::indirectIndexed, &Cpu::AND};
    opcodeTable[0x51] = {"EOR", 5, &Cpu::indirectIndexed, &Cpu::EOR};
    opcodeTable[0x71] = {"ADC", 5, &Cpu::indirectIndexed, &Cpu::ADC};
    opcodeTable[0x91] = {"STA", 6, &Cpu::indirectIndexed, &Cpu::STA};
    opcodeTable[0xB1] = {"LDA", 5, &Cpu::indirectIndexed, &Cpu::LDA};
    opcodeTable[0xD1] = {"CMP", 5, &Cpu::indirectIndexed, &Cpu::CMP};
    opcodeTable[0xF1] = {"SBC", 5, &Cpu::indirectIndexed, &Cpu::SBC};

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
        nesByte opcodeByte = bus->read(pc++);

        opcode = opcodeTable[opcodeByte];
        cyclesRemaining = opcode.cycles;
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



// address modes

bool Cpu::implied() {
    fetchedByte = a;
    return false;
}

bool Cpu::absolute() {
    nesWord loAddressByte = bus->read(pc++);
    nesWord hiAddressByte = bus->read(pc++);
    fetchedByte = bus->read((hiAddressByte << 8) | loAddressByte);
    return false;
}

bool Cpu::absoluteXIndexed() {
    //TODO
    //NOTE: CAN HAVE ADDITIONAL CYCLES!
    return false;
}

bool Cpu::absoluteYIndexed() {
    //TODO
    //NOTE: CAN HAVE ADDITIONAL CYCLES!
    return false;
}

bool Cpu::immediate() {
    fetchedByte = bus->read(pc++);
    return false;
}

bool Cpu::indirect() {
    //TODO
    return false;
}

bool Cpu::indexedIndirect() {
    //TODO
    return false;
}

bool Cpu::indirectIndexed() {
    //TODO
    //NOTE: CAN HAVE ADDITIONAL CYCLES!
    return false;
}

bool Cpu::relative() {
    //TODO
    return false;
}

bool Cpu::zeropage() {
    absoluteAddress = bus->read(pc++);
    fetchedByte = bus->read(absoluteAddress);
    return false;
}

bool Cpu::zeropageXIndexed() {
    absoluteAddress = (bus->read(pc++) + x) & 0xFF;
    fetchedByte = bus->read(absoluteAddress);
    return false;
}

bool Cpu::zeropageYIndexed() {
    absoluteAddress = (bus->read(pc++) + y) & 0xFF;
    fetchedByte = bus->read(absoluteAddress);
    return false;
}



// operations

bool Cpu::invalid() {
    // do nothing for invalid operations
    return false;
}

bool Cpu::BRK() {
    //TODO
    return false;
}

bool Cpu::RTI() {
    //TODO
    return false;
}

bool Cpu::RTS() {
    //TODO
    return false;
}

bool Cpu::PHP() {
    bus->write(0x0100 + sp, status | U_FLAG | B_FLAG);
    sp--;
    return false;
}

bool Cpu::CLC() {
    setStatusFlag(C_FLAG, 0);
    return false;
}

bool Cpu::PLP() {
    sp++;
    status = bus->read(0x0100 + sp) & (~U_FLAG) & (~B_FLAG);
    return false;
}

bool Cpu::SEC() {
    setStatusFlag(C_FLAG, 1);
    return false;
}

bool Cpu::PHA() {
    bus->write(0x0100 + sp, a);
    sp--;
    return false;
}

bool Cpu::CLI() {
    setStatusFlag(I_FLAG, 0);
    return false;
}

bool Cpu::PLA() {
    sp++;
    a = bus->read(0x0100 + sp);
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
    //TODO
    return false;
}

bool Cpu::BIT() {
    //TODO
    return false;
}

bool Cpu::JMP() {
    //TODO
    return false;
}

bool Cpu::STY() {
    //TODO
    return false;
}

bool Cpu::LDY() {
    //TODO
    //NOTE: CAN HAVE ADDITIONAL CYCLES!
    return false;
}

bool Cpu::CPY() {
    //TODO
    return false;
}

bool Cpu::CPX() {
    //TODO
    return false;
}

bool Cpu::ORA() {
    //TODO
    //NOTE: CAN HAVE ADDITIONAL CYCLES!
    return false;
}

bool Cpu::AND() {
    //TODO
    //NOTE: CAN HAVE ADDITIONAL CYCLES!
    return false;
}

bool Cpu::EOR() {
    //TODO
    //NOTE: CAN HAVE ADDITIONAL CYCLES!
    return false;
}

bool Cpu::ADC() {
    //TODO
    //NOTE: CAN HAVE ADDITIONAL CYCLES!
    return false;
}

bool Cpu::STA() {
    //TODO
    return false;
}

bool Cpu::LDA() {
    a = fetchedByte;
    setStatusFlag(N_FLAG, a & 0x80);
    setStatusFlag(Z_FLAG, a == 0x00);
    return true;
}

bool Cpu::CMP() {
    //TODO
    //NOTE: CAN HAVE ADDITIONAL CYCLES!
    return false;
}

bool Cpu::SBC() {
    //TODO
    //NOTE: CAN HAVE ADDITIONAL CYCLES!
    return false;
}

bool Cpu::ASL() {
    //TODO
    return false;
}

bool Cpu::ROL() {
    //TODO
    return false;
}

bool Cpu::LSR() {
    //TODO
    return false;
}

bool Cpu::ROR() {
    //TODO
    return false;
}

bool Cpu::STX() {
    bus->write(absoluteAddress, x);
    return false;
}

bool Cpu::LDX() {
    x = fetchedByte;
    setStatusFlag(N_FLAG, x & 0x80);
    setStatusFlag(Z_FLAG, x == 0x00);
    return true;
}

bool Cpu::DEC() {
    //TODO
    return false;
}

bool Cpu::INC() {
    //TODO
    return false;
}

bool Cpu::BPL() {
    //TODO
    //NOTE: MODIFY CYCLES DIRECTLY DUE TO WEIRD BEHAVIOR!
    return false;
}
bool Cpu::BMI() {
    //TODO
    //NOTE: MODIFY CYCLES DIRECTLY DUE TO WEIRD BEHAVIOR!
    return false;
}
bool Cpu::BVC() {
    //TODO
    //NOTE: MODIFY CYCLES DIRECTLY DUE TO WEIRD BEHAVIOR!
    return false;
}
bool Cpu::BVS() {
    //TODO
    //NOTE: MODIFY CYCLES DIRECTLY DUE TO WEIRD BEHAVIOR!
    return false;
}
bool Cpu::BCC() {
    //TODO
    //NOTE: MODIFY CYCLES DIRECTLY DUE TO WEIRD BEHAVIOR!
    return false;
}
bool Cpu::BCS() {
    //TODO
    //NOTE: MODIFY CYCLES DIRECTLY DUE TO WEIRD BEHAVIOR!
    return false;
}
bool Cpu::BNE() {
    //TODO
    //NOTE: MODIFY CYCLES DIRECTLY DUE TO WEIRD BEHAVIOR!
    return false;
}
bool Cpu::BEQ() {
    //TODO
    //NOTE: MODIFY CYCLES DIRECTLY DUE TO WEIRD BEHAVIOR!
    return false;
}
