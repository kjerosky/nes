#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <string>
#include <sstream>
#include <cstdlib>

#include "TextUtils.h"
#include "Nes.h"
#include "Cartridge.h"

const int WINDOW_WIDTH = 780;
const int WINDOW_HEIGHT = 480;

class Display : public olc::PixelGameEngine {

private:

    Nes* nes;
    Cartridge* cartridge;

    std::string filename;
    CpuInfo cpuInfo;


    enum DisplayRenderMode {
        SPRITES_INFO_MODE,
        CODE_MODE,
        GAME_MODE
    };
    DisplayRenderMode displayRenderMode;

    int selectedPaletteIndex;

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

        nes = new Nes(cartridge);

        displayRenderMode = SPRITES_INFO_MODE;

        selectedPaletteIndex = 0;

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        updateControllerStates();

        if (GetKey(olc::Key::ESCAPE).bPressed) {
            return false;
        } else if (GetKey(olc::Key::R).bPressed) {
            nes->reset();
        } else if (GetKey(olc::Key::I).bPressed) {
            nes->irq();
        } else if (GetKey(olc::Key::N).bPressed) {
            nes->nmi();
        } else if (GetKey(olc::Key::M).bPressed) {
            if (displayRenderMode == SPRITES_INFO_MODE) {
                displayRenderMode = CODE_MODE;
            } else if (displayRenderMode == CODE_MODE) {
                displayRenderMode = GAME_MODE;
            } else if (displayRenderMode == GAME_MODE) {
                displayRenderMode = SPRITES_INFO_MODE;
            }
        } else if (GetKey(olc::Key::ENTER).bPressed) {
            nes->executeNextInstruction();
        } else if (GetKey(olc::Key::F).bPressed) {
            nes->displayNextFrame();
        } else if (GetKey(olc::Key::SPACE).bPressed) {
            nes->toggleContinuousExecution();
        } else if (GetKey(olc::Key::P).bPressed) {
            selectedPaletteIndex = (selectedPaletteIndex + 1) % 8;
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

    void updateControllerStates() {
        nesByte controller1State = 0x00;
        controller1State |= (GetKey(olc::Key::K).bHeld ? 0x01 : 0x00);
        controller1State |= (GetKey(olc::Key::J).bHeld ? 0x02 : 0x00);
        controller1State |= (GetKey(olc::Key::L).bHeld ? 0x04 : 0x00);
        controller1State |= (GetKey(olc::Key::O).bHeld ? 0x08 : 0x00);
        controller1State |= (GetKey(olc::Key::W).bHeld ? 0x10 : 0x00);
        controller1State |= (GetKey(olc::Key::S).bHeld ? 0x20 : 0x00);
        controller1State |= (GetKey(olc::Key::A).bHeld ? 0x40 : 0x00);
        controller1State |= (GetKey(olc::Key::D).bHeld ? 0x80 : 0x00);

        // controller 2 is unimplemented for now
        nesByte controller2State = 0x00;

        nes->updateControllerStates(controller1State, controller2State);
    }

    void renderDisplay() {
        cpuInfo = nes->getCpuInfo();

        olc::Sprite* patternTable0 = nes->getPatternTable(0, selectedPaletteIndex);
        olc::Sprite* patternTable1 = nes->getPatternTable(1, selectedPaletteIndex);

        if (displayRenderMode == SPRITES_INFO_MODE) {
            Clear(olc::DARK_BLUE);

            drawPalettes(516, 338);
            DrawSprite(516, 348, patternTable0);
            DrawSprite(648, 348, patternTable1);
            DrawSprite(0, 0, nes->getScreen(), 2);
            drawSpritesInfo(520, 8, 40);
        } else if (displayRenderMode == CODE_MODE) {
            Clear(olc::DARK_BLUE);

            drawCpuData(520, 8);
            drawPalettes(516, 338);
            DrawSprite(516, 348, patternTable0);
            DrawSprite(648, 348, patternTable1);
            DrawSprite(0, 0, nes->getScreen(), 2);
        } else if (displayRenderMode == GAME_MODE) {
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

    void drawPalettes(int x, int y) {
        olc::Pixel* activePalettesColors = nes->getActivePalettesColors();
        for (int paletteIndex = 0; paletteIndex < 8; paletteIndex++) {
            for (int colorIndex = 0; colorIndex < 4; colorIndex++) {
                olc::Pixel color = activePalettesColors[paletteIndex * 4 + colorIndex];
                FillRect(x + paletteIndex * 32 + colorIndex * 6, y, 6, 6, color);
            }
        }

        DrawRect(x + selectedPaletteIndex * 32 - 1, y - 1, 4 * 6 + 1, 7, olc::GREEN);
    }

    void drawNameTableBytes(int x, int y) {
        nesByte* nameTable = nes->getNameTable(0);
        for (int row = 0; row < 32; row++) {
            for (int column = 0; column < 32; column++) {
                DrawString(
                    x + column * 16,
                    y + row * 16,
                    hex(nameTable[row * 32 + column], 2),
                    column % 2 == 0 ? olc::WHITE : olc::GREY,
                    1
                );
            }
        }
    }

    void drawNameTableTiles(int x, int y, olc::Sprite* backgroundPatternTable) {
        nesByte* nameTable = nes->getNameTable(0);
        for (int row = 0; row < 32; row++) {
            for (int column = 0; column < 32; column++) {
                nesByte tileId = nameTable[row * 32 + column];
                DrawPartialSprite(
                    x + column * 16,
                    y + row * 16,
                    backgroundPatternTable,
                    (tileId & 0x0F) * 8,
                    ((tileId >> 4) & 0x0F) * 8,
                    8,
                    8,
                    2
                );
            }
        }
    }

    void drawSpritesInfo(int x, int y, int rowCount) {
        nesByte* oamBytes = nes->getOamBytes();
        for (int row = 0; row < 64; row++) {
            int spriteX = oamBytes[3];
            int spriteY = oamBytes[0];

            if (row < rowCount) {
                DrawString(x, y + row * 8,
                    hex(row, 2)
                    + ": (" + std::to_string(spriteX) + ", " + std::to_string(spriteY)
                    + ") ID:" + hex(oamBytes[1], 2)
                    + " AT:" + hex(oamBytes[2], 2)
                );
            }

            oamBytes += 4;
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
