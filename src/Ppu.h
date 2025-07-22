#ifndef PPU_H
#define PPU_H

#include <SDL3/SDL.h>

#include "Cartridge.h"

class Ppu {

public:

    Ppu(Cartridge* cartridge, SDL_Texture* screen_texture, const SDL_PixelFormatDetails* pixel_format_details);
    ~Ppu();

    void clockTick();
    bool isFrameComplete();
    void acknowledgeFrameWasCompleted();
    bool checkAndResetNmiSignal();
    void reset();

    Uint8 cpuRead(Uint16 address, bool onlyRead = false);
    void cpuWrite(Uint16 address, Uint8 data);

    Uint8* getNameTable(int nameTableIndex);

    void writeToOam(Uint8 address, Uint8 data);
    Uint8* getOamBytes();

private:

    Cartridge* cartridge;

    Uint32 nesMainPalette[0x40];
    Uint8 activePalettesBytes[32];

    Uint32* screen_pixels;
    SDL_Texture* screen_texture;

    Uint8 nameTables[2][1024];

    int scanline;
    int cycle;
    bool frameIsComplete;
    bool wasNmiSignaled;

    class {
    public:
        Uint8 nameTableX : 1;
        Uint8 nameTableY : 1;
        Uint8 incrementMode : 1;
        Uint8 patternSprite : 1;
        Uint8 patternBackground : 1;
        Uint8 spriteSize : 1;
        Uint8 slaveMode : 1;
        Uint8 nmiEnabled : 1;

        void setData(Uint8 data) {
            nameTableX = (data >> 0) & 0x01;
            nameTableY = (data >> 1) & 0x01;
            incrementMode = (data >> 2) & 0x01;
            patternSprite = (data >> 3) & 0x01;
            patternBackground = (data >> 4) & 0x01;
            spriteSize = (data >> 5) & 0x01;
            slaveMode = (data >> 6) & 0x01;
            nmiEnabled = (data >> 7) & 0x01;
        }

        Uint8 getAsByte() {
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
        Uint8 greyscale : 1;
        Uint8 renderBackgroundLeft : 1;
        Uint8 renderSpritesLeft : 1;
        Uint8 renderBackground : 1;
        Uint8 renderSprites : 1;
        Uint8 emphasizeRed : 1;
        Uint8 emphasizeGreen : 1;
        Uint8 emphasizeBlue : 1;

        void setData(Uint8 data) {
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
        Uint8 verticalBlank : 1;
        Uint8 spriteZeroHit : 1;
        Uint8 spriteOverflow : 1;

        void setData(Uint8 data) {
            verticalBlank = (data >> 7) & 0x01;
            spriteZeroHit = (data >> 6) & 0x01;
            spriteOverflow = (data >> 5) & 0x01;
        }

        Uint8 getAsByte() {
            return
                (verticalBlank << 7) |
                (spriteZeroHit << 6) |
                (spriteOverflow << 5)
            ;
        }
    } ppuStatusRegister;

    bool ppuAddressLatchUseLoByte;
    Uint8 ppuDataBuffer;

    void initializePalette(const SDL_PixelFormatDetails* pixel_format_details);
    Uint32 getPaletteColor(int paletteIndex, int paletteColorIndex);

    Uint8 readViaPpuBus(Uint16 address, bool onlyRead = false);
    void writeViaPpuBus(Uint16 address, Uint8 data);

    class {
    public:
        Uint8 coarseX : 5;
        Uint8 coarseY : 5;
        Uint8 nameTableX : 1;
        Uint8 nameTableY : 1;
        Uint8 fineY : 3;

        void setData(Uint16 data) {
            coarseX = (data >> 0) & 0x1F;
            coarseY = (data >> 5) & 0x1F;
            nameTableX = (data >> 10) & 0x01;
            nameTableY = (data >> 11) & 0x01;
            fineY = (data >> 12) & 0x07;
        }

        Uint16 getAsWord() {
            return
                (fineY << 12) |
                (nameTableY << 11) |
                (nameTableX << 10) |
                (coarseY << 5) |
                coarseX
            ;
        }

        void increment(Uint16 amount) {
            setData((getAsWord() + amount) & 0x7FFF);
        }
    } vRamAddress, tRamAddress;

    Uint8 fineX;

    Uint8 nextBackgroundNameTableByte;
    Uint8 nextBackgroundAttributeByte;
    Uint8 nextBackgroundLsbpByte;
    Uint8 nextBackgroundMsbpByte;

    Uint16 backgroundPatternLsbShiftRegister;
    Uint16 backgroundPatternMsbShiftRegister;
    Uint16 backgroundAttributeLsbShiftRegister;
    Uint16 backgroundAttributeMsbShiftRegister;

    void incrementHorizontalScroll();
    void incrementVerticalScroll();
    void resetHorizontalScroll();
    void resetVerticalScroll();
    void transferBackgroundBytesToShiftRegisters();
    void updateShiftRegisters();

    struct spriteEntry {
        Uint8 y;
        Uint8 patternId;
        Uint8 attributes;
        Uint8 x;
    };
    spriteEntry oam[64];
    spriteEntry secondaryOam[8];
    Uint8* oamBytes = (Uint8*)oam;
    Uint8* secondaryOamBytes = (Uint8*)secondaryOam;
    Uint8 oamAddress;
    Uint8 spriteLsbShiftRegisters[8];
    Uint8 spriteMsbShiftRegisters[8];
    bool spriteZeroHitIsPossible;
    bool spriteZeroIsBeingRendered;

    void performSecondaryOamClearing();
    void performSpriteEvaluation();
    void performDataFetchesForSpritesToRenderNext();
    void loadSpriteShiftRegisters();

    void copy_pixel_data_to_screen_texture();
};

#endif
