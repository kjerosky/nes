#ifndef PPU_H
#define PPU_H

#include "olcPixelGameEngine.h"
#include "Cartridge.h"
#include "Types.h"

class Ppu {

public:

    Ppu(Cartridge* cartridge);
    ~Ppu();

    olc::Sprite* getScreen();
    void clockTick();
    bool isFrameComplete();
    void acknowledgeFrameWasCompleted();
    bool checkAndResetNmiSignal();

    nesByte cpuRead(nesWord address, bool onlyRead = false);
    void cpuWrite(nesWord address, nesByte data);

    olc::Sprite* getPatternTable(int patternTableIndex, int paletteIndex);
    olc::Pixel* getActivePalettesColors();

private:

    Cartridge* cartridge;

    olc::Pixel nesMainPalette[0x40];
    nesByte activePalettesBytes[32];
    olc::Pixel activePalettesColors[32];

    olc::Sprite screen;
    olc::Sprite patternTables[2];

    int scanline;
    int cycle;
    bool frameIsComplete;
    bool wasNmiSignaled;

    static const nesByte PPUCTRL_ENABLE_NMI_BIT = 1 << 7;
    static const nesByte PPUCTRL_SLAVE_MODE_BIT = 1 << 6;
    static const nesByte PPUCTRL_SPRITE_SIZE_BIT = 1 << 5;
    static const nesByte PPUCTRL_PATTERN_BACKGROUND_BIT = 1 << 4;
    static const nesByte PPUCTRL_PATTERN_SPRITE_BIT = 1 << 3;
    static const nesByte PPUCTRL_INCREMENT_MODE_BIT = 1 << 2;
    static const nesByte PPUCTRL_NAMETABLE_Y_BIT = 1 << 1;
    static const nesByte PPUCTRL_NAMETABLE_X_BIT = 1 << 0;
    nesByte ppuCtrlRegister;

    static const nesByte PPUMASK_EMPHASIZE_BLUE_BIT = 1 << 7;
    static const nesByte PPUMASK_EMPHASIZE_GREEN_BIT = 1 << 6;
    static const nesByte PPUMASK_EMPHASIZE_RED_BIT = 1 << 5;
    static const nesByte PPUMASK_RENDER_SPRITES_BIT = 1 << 4;
    static const nesByte PPUMASK_RENDER_BACKGROUND_BIT = 1 << 3;
    static const nesByte PPUMASK_RENDER_SPRITES_LEFT_BIT = 1 << 2;
    static const nesByte PPUMASK_RENDER_BACKGROUND_LEFT_BIT = 1 << 1;
    static const nesByte PPUMASK_GREYSCALE_BIT = 1 << 0;
    nesByte ppuMaskRegister;

    static const nesByte PPUSTATUS_VERTICAL_BLANK_BIT = 1 << 7;
    static const nesByte PPUSTATUS_SPRITE_ZERO_HIT_BIT = 1 << 6;
    static const nesByte PPUSTATUS_SPRITE_OVERFLOW_BIT = 1 << 5;
    nesByte ppuStatusRegister;

    bool ppuAddressLatchUseLoByte;
    nesByte ppuDataBuffer;
    nesWord ppuAddress;

    void initializePalette();
    olc::Pixel getPaletteColor(int paletteIndex, int paletteColorIndex);

    nesByte readViaPpuBus(nesWord address, bool onlyRead = false);
    void writeViaPpuBus(nesWord address, nesByte data);
};

#endif
