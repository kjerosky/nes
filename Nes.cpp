#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "TextUtils.h"
#include "Cpu.h"
#include "Bus.h"

// 16:9 aspect ratio with 2x nes resolution height
const int WINDOW_WIDTH = 853;
const int WINDOW_HEIGHT = 480;

class Display : public olc::PixelGameEngine {

private:

    const int CPU_DATA_RIGHT_SCREEN_EDGE_DISTANCE = 144;
    const int CPU_DATA_TOP_SCREEN_EDGE_DISTANCE = 8;

    Bus* bus;
    Cpu* cpu;

public:

    bool OnUserCreate() override {
        sAppName = "NES Emulator";

        bus = new Bus();
        cpu = new Cpu(bus);

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (GetKey(olc::Key::ESCAPE).bPressed) {
            return false;
        } else if (GetKey(olc::Key::SPACE).bPressed) {
            executeNextInstructions();
        }

        Clear(olc::DARK_BLUE);

        drawCpuData(ScreenWidth() - CPU_DATA_RIGHT_SCREEN_EDGE_DISTANCE, CPU_DATA_TOP_SCREEN_EDGE_DISTANCE);

        return true;
    }

    bool OnUserDestroy() override {
        delete bus;
        delete cpu;

        return true;
    }

    void executeNextInstructions() {
        do {
            cpu->clockTick();
        } while (!cpu->isCurrentInstructionComplete());
    }

    void drawCpuData(int x, int y) {
        CpuDebugInfo cpuDebugInfo = cpu->getDebugInfo();

        DrawString(x, y, "N", cpuDebugInfo.nFlag ? olc::YELLOW : olc::DARK_GREY);
        DrawString(x + 16, y, "V", cpuDebugInfo.vFlag ? olc::YELLOW : olc::DARK_GREY);
        DrawString(x + 32, y, "D", cpuDebugInfo.dFlag ? olc::YELLOW : olc::DARK_GREY);
        DrawString(x + 48, y, "I", cpuDebugInfo.iFlag ? olc::YELLOW : olc::DARK_GREY);
        DrawString(x + 64, y, "Z", cpuDebugInfo.zFlag ? olc::YELLOW : olc::DARK_GREY);
        DrawString(x + 80, y, "C", cpuDebugInfo.cFlag ? olc::YELLOW : olc::DARK_GREY);

        DrawString(x, y + 10, "PC=$" + hex(cpuDebugInfo.pc, 4));
        DrawString(x, y + 20, "SP=$" + hex(cpuDebugInfo.sp, 2));
        DrawString(x, y + 30, " A=$" + hex(cpuDebugInfo.a, 2) + " ("
            + std::to_string(cpuDebugInfo.a) + "/" + std::to_string((char)cpuDebugInfo.a) + ")");
        DrawString(x, y + 40, " X=$" + hex(cpuDebugInfo.x, 2) + " (" + std::to_string(cpuDebugInfo.x) + ")");
        DrawString(x, y + 50, " Y=$" + hex(cpuDebugInfo.y, 2) + " (" + std::to_string(cpuDebugInfo.y) + ")");
    }
};

int main(int argc, char* argv[]) {
    Display display;
    if (display.Construct(WINDOW_WIDTH, WINDOW_HEIGHT, 1, 1, true)) {
        display.Start();
    }

    return 0;
}
