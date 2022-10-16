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
    int (Cpu::*addressMode)(void);
    int (Cpu::*execute)(void);
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

    nesByte opcode;
    int cyclesRemaining;

    nesByte getStatusFlag(nesByte flag);
    void setStatusFlag(nesByte flag, bool value);

    nesByte fetchedByte;

    std::vector<Instruction> opcodeTable;
    void initializeOpcodeTable();

    int invalid();

    // address modes
    int implied();
    int absolute();
    int absoluteXIndexed();
    int absoluteYIndexed();
    int immediate();
    int indirect();
    int indexedIndirect(); // (preindexed or X-indexed, indirect)
    int indirectIndexed(); // (postindexed or indirect, Y-indexed)
    int relative();
    int zeropage();
    int zeropageXIndexed();
    int zeropageYIndexed();

    // operations
    int BRK();
    int RTI();
    int RTS();
    int PHP();
    int CLC();
    int PLP();
    int SEC();
    int PHA();
    int CLI();
    int PLA();
    int SEI();
    int DEY();
    int TYA();
    int TAY();
    int CLV();
    int INY();
    int CLD();
    int INX();
    int SED();
    int TXA();
    int TXS();
    int TAX();
    int TSX();
    int DEX();
    int NOP();
    int JSR();
    int BIT();
    int JMP();
    int STY();
    int LDY();
    int CPY();
    int CPX();
    int ORA();
    int AND();
    int EOR();
    int ADC();
    int STA();
    int LDA();
    int CMP();
    int SBC();
    int ASL();
    int ROL();
    int LSR();
    int ROR();
    int STX();
    int LDX();
    int DEC();
    int INC();
    int BPL();
    int BMI();
    int BVC();
    int BVS();
    int BCC();
    int BCS();
    int BNE();
    int BEQ();
};

#endif
