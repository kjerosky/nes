#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <sstream>
#include "TextUtils.h"
#include "Nes.h"

const int WINDOW_WIDTH = 768;
const int WINDOW_HEIGHT = 480;

class Display : public olc::PixelGameEngine {

private:

    Nes* nes;

    std::map<nesWord, std::string> disassembly;
    CpuInfo cpuInfo;


    enum DisplayRenderMode {
        SHOW_DEBUG_MODE,
        SHOW_SCREEN_RENDER_MODE
    };
    DisplayRenderMode displayRenderMode;

    olc::Sprite tempDisplay;

public:

    bool OnUserCreate() override {
        sAppName = "NES Emulator";

        std::stringstream mainCodeBytesStream;
        mainCodeBytesStream << "A2 05 8A 95 10 CA D0 FA A9 FF 85 10 4C 0C 80";
        std::stringstream irqCodeBytesStream;
        irqCodeBytesStream << "A9 03 38 E9 01 D0 FB 40";
        nes = new Nes(0x8000, mainCodeBytesStream, 0x80E0, irqCodeBytesStream);

        tempDisplay = olc::Sprite(256, 240);

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
                displayRenderMode = SHOW_SCREEN_RENDER_MODE;
            } else if (displayRenderMode == SHOW_SCREEN_RENDER_MODE) {
                displayRenderMode = SHOW_DEBUG_MODE;
            }
        } else if (GetKey(olc::Key::SPACE).bPressed) {
            nes->executeNextInstruction();
        }

        renderDisplay();

        return true;
    }

    bool OnUserDestroy() override {
        delete nes;

        return true;
    }

    void renderDisplay() {
        cpuInfo = nes->getCpuInfo();

        Clear(olc::DARK_BLUE);
        drawCpuData(520, 8);
        drawCode(520, 72, 25);

        if (displayRenderMode == SHOW_DEBUG_MODE) {
            drawPageData(8, 8, 0x0000);
            drawPageData(8, 152, 0x0100);
            drawPageData(8, 296, 0x8000);
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
    Display display;
    if (display.Construct(WINDOW_WIDTH, WINDOW_HEIGHT, 1, 1, true)) {
        display.Start();
    }

    return 0;
}
