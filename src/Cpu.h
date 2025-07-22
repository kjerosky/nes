#ifndef CPU_H
#define CPU_H

#include <string>
#include <map>
#include <vector>
#include <SDL3/SDL.h>

#include "Bus.h"

class Cpu;

typedef struct {
    Uint8 a;
    Uint8 x;
    Uint8 y;
    Uint8 sp;
    Uint8 status;
    Uint16 pc;

    bool nFlag;
    bool vFlag;
    bool dFlag;
    bool iFlag;
    bool zFlag;
    bool cFlag;
} CpuInfo;

typedef struct {
    std::string name;
    int cycles;
    bool (Cpu::*addressMode)(void);
    bool (Cpu::*execute)(void);
} Instruction;

class Cpu {

public:

    Cpu(Bus* bus);
    ~Cpu();

    void reset();
    void irq();
    void nmi();
    void clockTick();
    bool isCurrentInstructionComplete();
    std::map<Uint16, std::string> disassemble(Uint16 lowerAddress, Uint16 upperAddress);
    CpuInfo getInfo();

private:

    static const Uint8 N_FLAG = 1 << 7;
    static const Uint8 V_FLAG = 1 << 6;
    static const Uint8 U_FLAG = 1 << 5;
    static const Uint8 B_FLAG = 1 << 4;
    static const Uint8 D_FLAG = 1 << 3;
    static const Uint8 I_FLAG = 1 << 2;
    static const Uint8 Z_FLAG = 1 << 1;
    static const Uint8 C_FLAG = 1 << 0;

    Bus* bus;

    Uint8 a;
    Uint8 x;
    Uint8 y;
    Uint8 sp;
    Uint8 status;
    Uint16 pc;

    Instruction opcode;
    int cyclesRemaining;

    Uint8 getStatusFlag(Uint8 flag);
    void setStatusFlag(Uint8 flag, bool value);

    Uint8 fetchedByte;
    Uint16 absoluteAddress;
    Uint16 relativeAddress;

    bool (Cpu::*addressMode)(void);
    void fetch();

    std::vector<Instruction> opcodeTable;
    void initializeOpcodeTable();

    // address modes
    bool implied();
    bool absolute();
    bool absoluteXIndexed();
    bool absoluteYIndexed();
    bool immediate();
    bool indirect();
    bool xIndexedZeropageIndirect(); // (also called indexed indirect or preindexed or X-indexed, indirect)
    bool zeropageIndirectYIndexed(); // (also called indirect indexed or postindexed or indirect, Y-indexed)
    bool relative();
    bool zeropage();
    bool zeropageXIndexed();
    bool zeropageYIndexed();

    // operations
    bool invalid();
    bool BRK();
    bool RTI();
    bool RTS();
    bool PHP();
    bool CLC();
    bool PLP();
    bool SEC();
    bool PHA();
    bool CLI();
    bool PLA();
    bool SEI();
    bool DEY();
    bool TYA();
    bool TAY();
    bool CLV();
    bool INY();
    bool CLD();
    bool INX();
    bool SED();
    bool TXA();
    bool TXS();
    bool TAX();
    bool TSX();
    bool DEX();
    bool NOP();
    bool JSR();
    bool BIT();
    bool JMP();
    bool STY();
    bool LDY();
    bool CPY();
    bool CPX();
    bool ORA();
    bool AND();
    bool EOR();
    bool ADC();
    bool STA();
    bool LDA();
    bool CMP();
    bool SBC();
    bool ASL();
    bool ROL();
    bool LSR();
    bool ROR();
    bool STX();
    bool LDX();
    bool DEC();
    bool INC();
    bool BPL();
    bool BMI();
    bool BVC();
    bool BVS();
    bool BCC();
    bool BCS();
    bool BNE();
    bool BEQ();
};

#endif
