#ifndef CPU_H
#define CPU_H

typedef unsigned char byte;
typedef unsigned short word;

class Cpu {

public:

    Cpu();
    ~Cpu();

    int executeOneInstruction();
    void debugRegisters();

private:

    static const int RAM_SIZE = 65536;

    static const int N_BIT_POSITION = 7;
    static const int V_BIT_POSITION = 6;
    static const int D_BIT_POSITION = 3;
    static const int I_BIT_POSITION = 2;
    static const int Z_BIT_POSITION = 1;
    static const int C_BIT_POSITION = 0;

    unsigned char ram[RAM_SIZE];

    byte a;
    byte x;
    byte y;
    byte sp;
    word pc;

    bool nFlag;
    bool vFlag;
    bool dFlag;
    bool iFlag;
    bool zFlag;
    bool cFlag;

    void initialize();
    byte getFlagsAsByte();
};

#endif
