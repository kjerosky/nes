#ifndef CPU_H
#define CPU_H

#include <string>
#include <vector>
#include "Types.h"
#include "Bus.h"

class Cpu;

typedef struct {
    nesByte a;
    nesByte x;
    nesByte y;
    nesByte sp;
    nesByte status;
    nesWord pc;

    bool nFlag;
    bool vFlag;
    bool dFlag;
    bool iFlag;
    bool zFlag;
    bool cFlag;
} CpuDebugInfo;

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
    void clockTick();
    bool isCurrentInstructionComplete();
    CpuDebugInfo getDebugInfo();

private:

    static const nesByte N_FLAG = 1 << 7;
    static const nesByte V_FLAG = 1 << 6;
    static const nesByte U_FLAG = 1 << 5;
    static const nesByte B_FLAG = 1 << 4;
    static const nesByte D_FLAG = 1 << 3;
    static const nesByte I_FLAG = 1 << 2;
    static const nesByte Z_FLAG = 1 << 1;
    static const nesByte C_FLAG = 1 << 0;

    Bus* bus;

    nesByte a;
    nesByte x;
    nesByte y;
    nesByte sp;
    nesByte status;
    nesWord pc;

    Instruction opcode;
    int cyclesRemaining;

    nesByte getStatusFlag(nesByte flag);
    void setStatusFlag(nesByte flag, bool value);

    nesByte fetchedByte;
    nesWord absoluteAddress;
    nesWord relativeAddress;

    std::vector<Instruction> opcodeTable;
    void initializeOpcodeTable();

    // address modes
    bool implied();
    bool absolute();
    bool absoluteXIndexed();
    bool absoluteYIndexed();
    bool immediate();
    bool indirect();
    bool indexedIndirect(); // (preindexed or X-indexed, indirect)
    bool indirectIndexed(); // (postindexed or indirect, Y-indexed)
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
