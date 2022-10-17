#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <sstream>
#include "TextUtils.h"
#include "Cpu.h"
#include "Bus.h"

const int WINDOW_WIDTH = 768;
const int WINDOW_HEIGHT = 480;

class Display : public olc::PixelGameEngine {

private:

    Bus* bus;
    Cpu* cpu;

    enum DisplayRenderMode {
        SHOW_MEMORY_RENDER_MODE,
        SHOW_SCREEN_RENDER_MODE
    };
    DisplayRenderMode displayRenderMode;

    olc::Sprite tempDisplay;

public:

    bool OnUserCreate() override {
        sAppName = "NES Emulator";

        bus = new Bus();
        bus->ram[0xFFFC] = 0x00;
        bus->ram[0xFFFD] = 0x80;
        loadProgram();

        cpu = new Cpu(bus);

        tempDisplay = olc::Sprite(256, 240);

        displayRenderMode = SHOW_MEMORY_RENDER_MODE;

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (GetKey(olc::Key::ESCAPE).bPressed) {
            return false;
        } else if (GetKey(olc::Key::R).bPressed) {
            cpu->reset();
        } else if (GetKey(olc::Key::M).bPressed) {
            if (displayRenderMode == SHOW_MEMORY_RENDER_MODE) {
                displayRenderMode = SHOW_SCREEN_RENDER_MODE;
            } else if (displayRenderMode == SHOW_SCREEN_RENDER_MODE) {
                displayRenderMode = SHOW_MEMORY_RENDER_MODE;
            }
        } else if (GetKey(olc::Key::SPACE).bPressed) {
            executeNextInstructions();
        }

        renderDisplay();

        return true;
    }

    bool OnUserDestroy() override {
        delete bus;
        delete cpu;

        return true;
    }

    void loadProgram() {
        std::stringstream ss;
        ss << "38 78 F8 EA EA EA 18 58 D8 B8";
        nesWord ramOffset = 0x8000;
        while (!ss.eof()) {
            std::string programByte;
            ss >> programByte;
            bus->ram[ramOffset++] = (nesByte)std::stoul(programByte, nullptr, 16);
        }
    }

    void renderDisplay() {
        Clear(olc::DARK_BLUE);
        drawCpuData(520, 8);

        if (displayRenderMode == SHOW_MEMORY_RENDER_MODE) {
            drawPageData(8, 8, 0x0000);
            drawPageData(8, 152, 0x8000);
        } else if (displayRenderMode == SHOW_SCREEN_RENDER_MODE) {
            //TODO fake some noise for now...need to get this from the ppu in the future
            for (int y = 0; y < 240; y++) {
                for (int x = 0; x < 256; x++) {
                    tempDisplay.SetPixel(x, y, rand() % 2 ? olc::BLACK : olc::Pixel(230, 230, 230));
                }
            }
            DrawSprite(0, 0, &tempDisplay, 2);
        }
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

    void drawPageData(int x, int y, nesWord pageStartAddress) {
        for (int row = 0; row < 16; row++) {
            DrawString(x, y + row * 8, "$" + hex(pageStartAddress + row * 0x10, 4) + ":");

            for (int column = 0; column < 16; column++) {
                nesByte currentByte = bus->ram[pageStartAddress + row * 16 + column];
                DrawString(x + 7 * 8 + column * 3 * 8, y + row * 8, hex(currentByte, 2));
            }
        }
    }
};

int main(int argc, char* argv[]) {
    Display display;
    if (display.Construct(WINDOW_WIDTH, WINDOW_HEIGHT, 1, 1, true)) {
        display.Start();
    }

    return 0;
}
