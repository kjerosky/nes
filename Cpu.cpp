#include <iostream>
#include <iomanip>
#include <algorithm>
#include "Cpu.h"

using namespace std;

Cpu::Cpu() {
    initialize();
}

Cpu::~Cpu() {
    //TODO
}

void Cpu::debugRegisters() {
    byte flags = getFlagsAsByte();
    bitset<8> flagsBits(flags);

    cout << uppercase << setfill('0')
        << "PC=$" << setw(4) << right << hex << (int)pc
        << " SP=$" << setw(2) << right << hex << (int)sp
        << " A=$" << setw(2) << right << hex << (int)a
        << " X=$" << setw(2) << right << hex << (int)x
        << " Y=$" << setw(2) << right << hex << (int)y
        << " FLAGS=$" << setw(2) << right << hex << (int)flags << "=" << flagsBits << "="
        << "N" << (int)nFlag << "/V" << (int)vFlag << "/D" << (int)dFlag
        << "/I" << (int)iFlag << "/Z" << (int)zFlag << "/C" << (int)cFlag
    << endl;
}

void Cpu::initialize() {
    fill(ram, ram + RAM_SIZE, 0);

    a = 0;
    x = 0;
    y = 0;
    sp = 0xFF;
    pc = 0;

    nFlag = 0;
    vFlag = 0;
    dFlag = 0;
    iFlag = 0;
    zFlag = 0;
    cFlag = 0;
}

int Cpu::executeOneInstruction() {
    int cycleCount = 0;

    byte currentInstruction = ram[pc++];
    switch (currentInstruction) {
        // LDA
        case 0xA9:
        case 0xA5:
        case 0xB5:
        case 0xAD:
        case 0xBD:
        case 0xB9:
        case 0xA1:
        case 0xB1: {
            byte valueToLoad = 0;
            switch (currentInstruction) {
                case 0xA9: // immediate
                    //TODO
                    break;
                case 0xA5: // zero page
                    //TODO
                    break;
                case 0xB5: // zero page, X
                    //TODO
                    break;
                case 0xAD: // absolute
                    //TODO
                    break;
                case 0xBD: // absolute, X
                    //TODO
                    break;
                case 0xB9: // absolute, Y
                    //TODO
                    break;
                case 0xA1: // indirect, X
                    //TODO
                    break;
                case 0xB1: // indirect, Y
                    //TODO
                    break;
            }

            a = valueToLoad;
        } break;

        default:
            cerr << "[ERROR] cpu attempted to execute an unknown opcode: $"
                << uppercase << setfill('0') << setw(2) << right << hex << (int)currentInstruction << endl;
    }

    if (cycleCount == 0) {
        cerr << "[ERROR] instruction $"
            << uppercase << setfill('0') << setw(2) << right << hex << (int)currentInstruction
            << " has an unset cycle count!" << endl;
    }

    return cycleCount;
}

byte Cpu::getFlagsAsByte() {
    return
        (nFlag << N_BIT_POSITION) |
        (vFlag << V_BIT_POSITION) |
        (dFlag << D_BIT_POSITION) |
        (iFlag << I_BIT_POSITION) |
        (zFlag << Z_BIT_POSITION) |
        (cFlag << C_BIT_POSITION)
    ;
}