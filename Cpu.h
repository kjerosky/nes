#ifndef CPU_H
#define CPU_H

#include "Types.h"
#include "Bus.h"

typedef struct {
    byte a;
    byte x;
    byte y;
    byte sp;
    byte status;
    word pc;

    bool nFlag;
    bool vFlag;
    bool dFlag;
    bool iFlag;
    bool zFlag;
    bool cFlag;
} CpuDebugInfo;

class Cpu {

public:

    Cpu(Bus* bus);
    ~Cpu();

    void clockTick();
    bool isCurrentInstructionComplete();
    CpuDebugInfo getDebugInfo();

private:

    static const byte N_FLAG = 1 << 7;
    static const byte V_FLAG = 1 << 6;
    static const byte D_FLAG = 1 << 3;
    static const byte I_FLAG = 1 << 2;
    static const byte Z_FLAG = 1 << 1;
    static const byte C_FLAG = 1 << 0;

    Bus* bus;

    byte a;
    byte x;
    byte y;
    byte sp;
    byte status;
    word pc;

    byte opcode;
    int cyclesRemaining;

    void initialize();

    byte getStatusFlag(byte flag);
    void setStatusFlag(byte flag);
    void clearStatusFlag(byte flag);
};

#endif
