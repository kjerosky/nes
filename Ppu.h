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
    void reset();

    nesByte cpuRead(nesWord address, bool onlyRead = false);
    void cpuWrite(nesWord address, nesByte data);

    olc::Sprite* getPatternTable(int patternTableIndex, int paletteIndex);
    olc::Pixel* getActivePalettesColors();
    nesByte* getNameTable(int nameTableIndex);

    void writeToOam(nesByte address, nesByte data);
    nesByte* getOamBytes();
private:

    Cartridge* cartridge;

    olc::Pixel nesMainPalette[0x40];
    nesByte activePalettesBytes[32];
    olc::Pixel activePalettesColors[32];

    olc::Sprite screen;
    olc::Sprite patternTables[2];

    nesByte nameTables[2][1024];

    int scanline;
    int cycle;
    bool frameIsComplete;
    bool wasNmiSignaled;

    class {
    public:
        nesByte nameTableX : 1;
        nesByte nameTableY : 1;
        nesByte incrementMode : 1;
        nesByte patternSprite : 1;
        nesByte patternBackground : 1;
        nesByte spriteSize : 1;
        nesByte slaveMode : 1;
        nesByte nmiEnabled : 1;

        void setData(nesByte data) {
            nameTableX = (data >> 0) & 0x01;
            nameTableY = (data >> 1) & 0x01;
            incrementMode = (data >> 2) & 0x01;
            patternSprite = (data >> 3) & 0x01;
            patternBackground = (data >> 4) & 0x01;
            spriteSize = (data >> 5) & 0x01;
            slaveMode = (data >> 6) & 0x01;
            nmiEnabled = (data >> 7) & 0x01;
        }

        nesByte getAsByte() {
            return
                (nameTableX << 0) |
                (nameTableY << 1) |
                (incrementMode << 2) |
                (patternSprite << 3) |
                (patternBackground << 4) |
                (spriteSize << 5) |
                (slaveMode << 6) |
                (nmiEnabled << 7)
            ;
        }
    } ppuCtrlRegister;

    class {
    public:
        nesByte greyscale : 1;
        nesByte renderBackgroundLeft : 1;
        nesByte renderSpritesLeft : 1;
        nesByte renderBackground : 1;
        nesByte renderSprites : 1;
        nesByte emphasizeRed : 1;
        nesByte emphasizeGreen : 1;
        nesByte emphasizeBlue : 1;

        void setData(nesByte data) {
            greyscale = (data >> 0) & 0x01;
            renderBackgroundLeft = (data >> 1) & 0x01;
            renderSpritesLeft = (data >> 2) & 0x01;
            renderBackground = (data >> 3) & 0x01;
            renderSprites = (data >> 4) & 0x01;
            emphasizeRed = (data >> 5) & 0x01;
            emphasizeGreen = (data >> 6) & 0x01;
            emphasizeBlue = (data >> 7) & 0x01;
        }
    } ppuMaskRegister;

    class {
    public:
        nesByte verticalBlank : 1;
        nesByte spriteZeroHit : 1;
        nesByte spriteOverflow : 1;

        void setData(nesByte data) {
            verticalBlank = (data >> 7) & 0x01;
            spriteZeroHit = (data >> 6) & 0x01;
            spriteOverflow = (data >> 5) & 0x01;
        }

        nesByte getAsByte() {
            return
                (verticalBlank << 7) |
                (spriteZeroHit << 6) |
                (spriteOverflow << 5)
            ;
        }
    } ppuStatusRegister;

    bool ppuAddressLatchUseLoByte;
    nesByte ppuDataBuffer;

    void initializePalette();
    olc::Pixel getPaletteColor(int paletteIndex, int paletteColorIndex);

    nesByte readViaPpuBus(nesWord address, bool onlyRead = false);
    void writeViaPpuBus(nesWord address, nesByte data);

    class {
    public:
        nesByte coarseX : 5;
        nesByte coarseY : 5;
        nesByte nameTableX : 1;
        nesByte nameTableY : 1;
        nesByte fineY : 3;

        void setData(nesWord data) {
            coarseX = (data >> 0) & 0x1F;
            coarseY = (data >> 5) & 0x1F;
            nameTableX = (data >> 10) & 0x01;
            nameTableY = (data >> 11) & 0x01;
            fineY = (data >> 12) & 0x07;
        }

        nesWord getAsWord() {
            return
                (fineY << 12) |
                (nameTableY << 11) |
                (nameTableX << 10) |
                (coarseY << 5) |
                coarseX
            ;
        }

        void increment(nesWord amount) {
            setData((getAsWord() + amount) & 0x7FFF);
        }
    } vRamAddress, tRamAddress;

    nesByte fineX;

    nesByte nextBackgroundNameTableByte;
    nesByte nextBackgroundAttributeByte;
    nesByte nextBackgroundLsbpByte;
    nesByte nextBackgroundMsbpByte;

    nesWord backgroundPatternLsbShiftRegister;
    nesWord backgroundPatternMsbShiftRegister;
    nesWord backgroundAttributeLsbShiftRegister;
    nesWord backgroundAttributeMsbShiftRegister;

    void incrementHorizontalScroll();
    void incrementVerticalScroll();
    void resetHorizontalScroll();
    void resetVerticalScroll();
    void transferBackgroundBytesToShiftRegisters();
    void updateBackgroundShiftRegisters();

    struct {
        nesByte y;
        nesByte patternId;
        nesByte attributes;
        nesByte x;
    } oam[64];
    nesByte* oamBytes = (nesByte*)oam;
    nesByte oamAddress;
};

#endif
