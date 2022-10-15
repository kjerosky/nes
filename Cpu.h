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
    int (Cpu::*addressMode)();
    int (Cpu::*execute)();
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

    std::vector<Instruction> opcodeTable;
    void initializeOpcodeTable();

    int implied();

    int invalid();
};

#endif
