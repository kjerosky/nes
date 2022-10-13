#include <iostream>
#include "Cpu.h"

int main(int argc, char* argv[]) {
    Cpu cpu;
    cpu.debugRegisters();
    cpu.executeOneInstruction();

    std::cout << "Completed successfully!" << std::endl;
    return 0;
}
