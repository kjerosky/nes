#ifndef CPU_H
#define CPU_H

#include "Types.h"
#include "Bus.h"

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

class Cpu {

public:

    Cpu(Bus* bus);
    ~Cpu();

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

    void initialize();

    nesByte getStatusFlag(nesByte flag);
    void setStatusFlag(nesByte flag, bool value);
};

#endif
