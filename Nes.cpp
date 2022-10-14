#include <iostream>
#include <curses.h>
#include <SDL.h>
#include "Cpu.h"

using namespace std;

void printCpuDebugInfo(Cpu* cpu) {
    CpuDebugInfo cpuDebugInfo = cpu->getDebugInfo();

    bitset<8> statusBits(cpuDebugInfo.status);

    printw("PC=$%04X SP=$%02X A=$%02X X=$%02X Y=$%02X FLAGS=$%02X=%s/N=%d/V=%d/D=%d/I=%d/Z=%d/C=%d\n",
        (int)cpuDebugInfo.pc,
        (int)cpuDebugInfo.sp,
        (int)cpuDebugInfo.a,
        (int)cpuDebugInfo.x,
        (int)cpuDebugInfo.y,
        (int)cpuDebugInfo.status,
        statusBits.to_string().c_str(),
        (int)cpuDebugInfo.nFlag,
        (int)cpuDebugInfo.vFlag,
        (int)cpuDebugInfo.dFlag,
        (int)cpuDebugInfo.iFlag,
        (int)cpuDebugInfo.zFlag,
        (int)cpuDebugInfo.cFlag
    );
}

int main(int argc, char* argv[]) {
    initscr();
    SDL_Init(SDL_INIT_VIDEO);

    Bus bus;
    Cpu cpu(&bus);

    bool isRunning = true;
    bool shouldUpdateDisplay = true;
    while (isRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            isRunning = false;
                            break;

                        case SDLK_SPACE:
                            do {
                                cpu.clockTick();
                            } while (!cpu.isCurrentInstructionComplete());

                            shouldUpdateDisplay = true;
                            break;
                    }
                    break;
            }
        }

        if (shouldUpdateDisplay) {
            clear();
            move(1, 2);
            printCpuDebugInfo(&cpu);
            refresh();

            shouldUpdateDisplay = false;
        }
    }

    endwin();
    SDL_Quit();

    cout << "Completed successfully!" << endl;
    return 0;
}
