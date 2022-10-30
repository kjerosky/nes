#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <string>
#include <sstream>
#include <cstdlib>

#include "TextUtils.h"
#include "Nes.h"
#include "Cartridge.h"

const int WINDOW_WIDTH = 768;
const int WINDOW_HEIGHT = 480;

class Display : public olc::PixelGameEngine {

private:

    Nes* nes;
    Cartridge* cartridge;

    std::string filename;
    std::map<nesWord, std::string> disassembly;
    CpuInfo cpuInfo;


    enum DisplayRenderMode {
        SHOW_DEBUG_MODE,
        SHOW_ONLY_SCREEN_MODE
    };
    DisplayRenderMode displayRenderMode;

public:

    Display(std::string filename) {
        this->filename = filename;
    }

    bool OnUserCreate() override {
        sAppName = "NES Emulator";

        cartridge = new Cartridge(filename);
        if (!cartridge->isValid()) {
            exit(EXIT_FAILURE);
        }

        std::stringstream mainCodeBytesStream;
        mainCodeBytesStream << "A2 05 8A 95 10 CA D0 FA A9 FF 85 10 4C 0C 80";
        std::stringstream irqCodeBytesStream;
        irqCodeBytesStream << "A9 03 38 E9 01 D0 FB 40";
        nes = new Nes(0x8000, mainCodeBytesStream, 0x80E0, irqCodeBytesStream);

        disassembly = nes->disassemble(0x0000, 0xFFFF);

        displayRenderMode = SHOW_DEBUG_MODE;

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        if (GetKey(olc::Key::ESCAPE).bPressed) {
            return false;
        } else if (GetKey(olc::Key::R).bPressed) {
            nes->reset();
        } else if (GetKey(olc::Key::I).bPressed) {
            nes->irq();
        } else if (GetKey(olc::Key::M).bPressed) {
            if (displayRenderMode == SHOW_DEBUG_MODE) {
                displayRenderMode = SHOW_ONLY_SCREEN_MODE;
            } else if (displayRenderMode == SHOW_ONLY_SCREEN_MODE) {
                displayRenderMode = SHOW_DEBUG_MODE;
            }
        } else if (GetKey(olc::Key::ENTER).bPressed) {
            nes->executeNextInstruction();
        } else if (GetKey(olc::Key::F).bPressed) {
            nes->displayNextFrame();
        } else if (GetKey(olc::Key::SPACE).bPressed) {
            nes->toggleContinuousExecution();
        }

        nes->processTimeElapsed(fElapsedTime);
        renderDisplay();

        return true;
    }

    bool OnUserDestroy() override {
        delete nes;
        delete cartridge;

        return true;
    }

    void renderDisplay() {
        cpuInfo = nes->getCpuInfo();

        if (displayRenderMode == SHOW_DEBUG_MODE) {
            Clear(olc::DARK_BLUE);

            drawCpuData(520, 8);
            drawCode(520, 72, 25);
            DrawSprite(0, 0, nes->getScreen(), 2);
        } else if (displayRenderMode == SHOW_ONLY_SCREEN_MODE) {
            Clear(olc::BLACK);

            int screenCenteredX = (WINDOW_WIDTH - nes->getScreen()->width * 2) / 2;
            DrawSprite(screenCenteredX, 0, nes->getScreen(), 2);
        }
    }

    void drawCpuData(int x, int y) {
        DrawString(x, y, "N", cpuInfo.nFlag ? olc::YELLOW : olc::DARK_GREY);
        DrawString(x + 16, y, "V", cpuInfo.vFlag ? olc::YELLOW : olc::DARK_GREY);
        DrawString(x + 32, y, "D", cpuInfo.dFlag ? olc::YELLOW : olc::DARK_GREY);
        DrawString(x + 48, y, "I", cpuInfo.iFlag ? olc::YELLOW : olc::DARK_GREY);
        DrawString(x + 64, y, "Z", cpuInfo.zFlag ? olc::YELLOW : olc::DARK_GREY);
        DrawString(x + 80, y, "C", cpuInfo.cFlag ? olc::YELLOW : olc::DARK_GREY);

        DrawString(x, y + 10, "PC=$" + hex(cpuInfo.pc, 4));
        DrawString(x, y + 20, "SP=$" + hex(cpuInfo.sp, 2));
        DrawString(x, y + 30, " A=$" + hex(cpuInfo.a, 2) + " ("
            + std::to_string(cpuInfo.a) + "/" + std::to_string((char)cpuInfo.a) + ")");
        DrawString(x, y + 40, " X=$" + hex(cpuInfo.x, 2) + " (" + std::to_string(cpuInfo.x) + ")");
        DrawString(x, y + 50, " Y=$" + hex(cpuInfo.y, 2) + " (" + std::to_string(cpuInfo.y) + ")");
    }

    void drawPageData(int x, int y, nesWord pageStartAddress) {
        nesByte* cpuRam = nes->getCpuRam();
        for (int row = 0; row < 16; row++) {
            DrawString(x, y + row * 8, "$" + hex(pageStartAddress + row * 0x10, 4) + ":");

            for (int column = 0; column < 16; column++) {
                nesByte currentByte = cpuRam[pageStartAddress + row * 16 + column];
                DrawString(x + 7 * 8 + column * 3 * 8, y + row * 8, hex(currentByte, 2));
            }
        }
    }

    void drawCode(int x, int y, int instructionCount) {
        auto iterator = disassembly.find(cpuInfo.pc);
        if (iterator == disassembly.end()) {
            DrawString(x, y, "ERROR: NO DISASSEMBLY FOR $" + hex(cpuInfo.pc, 4), olc::RED);
            return;
        }

        for (int i = 0; i < instructionCount / 2; i++) {
            iterator--;
        }

        for (int row = 0; row < instructionCount; row++, iterator++) {
            DrawString(x, y + row * 8, (*iterator).second, (*iterator).first == cpuInfo.pc ? olc::GREEN : olc::WHITE);
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Display display(argv[1]);
    if (display.Construct(WINDOW_WIDTH, WINDOW_HEIGHT, 1, 1, true)) {
        display.Start();
    }

    return 0;
}
