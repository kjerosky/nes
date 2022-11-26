#include "Ppu.h"

Ppu::Ppu(Cartridge* cartridge) {
    this->cartridge = cartridge;

    screen = olc::Sprite(256, 240);
    patternTables[0] = olc::Sprite(128, 128);
    patternTables[1] = olc::Sprite(128, 128);

    initializePalette();

    reset();
}

Ppu::~Ppu() {
    // do nothing
}

olc::Sprite* Ppu::getScreen() {
    return &screen;
}

void Ppu::initializePalette() {
    nesMainPalette[0x00] = olc::Pixel(84, 84, 84);
    nesMainPalette[0x01] = olc::Pixel(0, 30, 116);
    nesMainPalette[0x02] = olc::Pixel(8, 16, 144);
    nesMainPalette[0x03] = olc::Pixel(48, 0, 136);
    nesMainPalette[0x04] = olc::Pixel(68, 0, 100);
    nesMainPalette[0x05] = olc::Pixel(92, 0, 48);
    nesMainPalette[0x06] = olc::Pixel(84, 4, 0);
    nesMainPalette[0x07] = olc::Pixel(60, 24, 0);
    nesMainPalette[0x08] = olc::Pixel(32, 42, 0);
    nesMainPalette[0x09] = olc::Pixel(8, 58, 0);
    nesMainPalette[0x0A] = olc::Pixel(0, 64, 0);
    nesMainPalette[0x0B] = olc::Pixel(0, 60, 0);
    nesMainPalette[0x0C] = olc::Pixel(0, 50, 60);
    nesMainPalette[0x0D] = olc::Pixel(0, 0, 0);
    nesMainPalette[0x0E] = olc::Pixel(0, 0, 0);
    nesMainPalette[0x0F] = olc::Pixel(0, 0, 0);

    nesMainPalette[0x10] = olc::Pixel(152, 150, 152);
    nesMainPalette[0x11] = olc::Pixel(8, 76, 196);
    nesMainPalette[0x12] = olc::Pixel(48, 50, 236);
    nesMainPalette[0x13] = olc::Pixel(92, 30, 228);
    nesMainPalette[0x14] = olc::Pixel(136, 20, 176);
    nesMainPalette[0x15] = olc::Pixel(160, 20, 100);
    nesMainPalette[0x16] = olc::Pixel(152, 34, 32);
    nesMainPalette[0x17] = olc::Pixel(120, 60, 0);
    nesMainPalette[0x18] = olc::Pixel(84, 90, 0);
    nesMainPalette[0x19] = olc::Pixel(40, 114, 0);
    nesMainPalette[0x1A] = olc::Pixel(8, 124, 0);
    nesMainPalette[0x1B] = olc::Pixel(0, 118, 40);
    nesMainPalette[0x1C] = olc::Pixel(0, 102, 120);
    nesMainPalette[0x1D] = olc::Pixel(0, 0, 0);
    nesMainPalette[0x1E] = olc::Pixel(0, 0, 0);
    nesMainPalette[0x1F] = olc::Pixel(0, 0, 0);

    nesMainPalette[0x20] = olc::Pixel(236, 238, 236);
    nesMainPalette[0x21] = olc::Pixel(76, 154, 236);
    nesMainPalette[0x22] = olc::Pixel(120, 124, 236);
    nesMainPalette[0x23] = olc::Pixel(176, 98, 236);
    nesMainPalette[0x24] = olc::Pixel(228, 84, 236);
    nesMainPalette[0x25] = olc::Pixel(236, 88, 180);
    nesMainPalette[0x26] = olc::Pixel(236, 106, 100);
    nesMainPalette[0x27] = olc::Pixel(212, 136, 32);
    nesMainPalette[0x28] = olc::Pixel(160, 170, 0);
    nesMainPalette[0x29] = olc::Pixel(116, 196, 0);
    nesMainPalette[0x2A] = olc::Pixel(76, 208, 32);
    nesMainPalette[0x2B] = olc::Pixel(56, 204, 108);
    nesMainPalette[0x2C] = olc::Pixel(56, 180, 204);
    nesMainPalette[0x2D] = olc::Pixel(60, 60, 60);
    nesMainPalette[0x2E] = olc::Pixel(0, 0, 0);
    nesMainPalette[0x2F] = olc::Pixel(0, 0, 0);

    nesMainPalette[0x30] = olc::Pixel(236, 238, 236);
    nesMainPalette[0x31] = olc::Pixel(168, 204, 236);
    nesMainPalette[0x32] = olc::Pixel(188, 188, 236);
    nesMainPalette[0x33] = olc::Pixel(212, 178, 236);
    nesMainPalette[0x34] = olc::Pixel(236, 174, 236);
    nesMainPalette[0x35] = olc::Pixel(236, 174, 212);
    nesMainPalette[0x36] = olc::Pixel(236, 180, 176);
    nesMainPalette[0x37] = olc::Pixel(228, 196, 144);
    nesMainPalette[0x38] = olc::Pixel(204, 210, 120);
    nesMainPalette[0x39] = olc::Pixel(180, 222, 120);
    nesMainPalette[0x3A] = olc::Pixel(168, 226, 144);
    nesMainPalette[0x3B] = olc::Pixel(152, 226, 180);
    nesMainPalette[0x3C] = olc::Pixel(160, 214, 228);
    nesMainPalette[0x3D] = olc::Pixel(160, 162, 160);
    nesMainPalette[0x3E] = olc::Pixel(0, 0, 0);
    nesMainPalette[0x3F] = olc::Pixel(0, 0, 0);
}

olc::Pixel Ppu::getPaletteColor(int paletteIndex, int paletteColorIndex) {
    int mainNesPaletteIndex = readViaPpuBus(0x3F00 + paletteIndex * 4 + paletteColorIndex, true);
    return nesMainPalette[mainNesPaletteIndex];
}

bool Ppu::isFrameComplete() {
    return frameIsComplete;
}

void Ppu::acknowledgeFrameWasCompleted() {
    frameIsComplete = false;
}

bool Ppu::checkAndResetNmiSignal() {
    bool nmiRaised = wasNmiSignaled;
    wasNmiSignaled = false;
    return nmiRaised;
}

void Ppu::reset() {
    scanline = 0;
    cycle = 0;
    frameIsComplete = false;
    wasNmiSignaled = false;

    ppuCtrlRegister.setData(0x00);
    ppuMaskRegister.setData(0x00);
    ppuStatusRegister.setData(0x00);

    ppuAddressLatchUseLoByte = false;
    ppuDataBuffer = 0x00;

    vRamAddress.setData(0x0000);
    tRamAddress.setData(0x0000);

    fineX = 0x00;;

    nextBackgroundNameTableByte = 0x00;
    nextBackgroundAttributeByte = 0x00;
    nextBackgroundLsbpByte = 0x00;
    nextBackgroundMsbpByte = 0x00;

    backgroundPatternLsbShiftRegister = 0x0000;
    backgroundPatternMsbShiftRegister = 0x0000;
    backgroundAttributeLsbShiftRegister = 0x0000;
    backgroundAttributeMsbShiftRegister = 0x0000;

    oamAddress = 0x00;
    spriteZeroHitIsPossible = false;
    spriteZeroIsBeingRendered = false;
}

olc::Sprite* Ppu::getPatternTable(int patternTableIndex, int paletteIndex) {
    for (nesWord tileRow = 0; tileRow < 16; tileRow++) {
        for (nesWord tileColumn = 0; tileColumn < 16; tileColumn++) {
            nesWord patternTableOffset = tileRow * 256 + tileColumn * 16;
            for (nesWord pixelRow = 0; pixelRow < 8; pixelRow++) {
                nesByte lineLsbpByte = readViaPpuBus(
                    patternTableIndex * 0x1000 + patternTableOffset + pixelRow, true);
                nesByte lineMsbpByte = readViaPpuBus(
                    patternTableIndex * 0x1000 + patternTableOffset + pixelRow + 8, true);

                for (nesByte pixelColumn = 0; pixelColumn < 8; pixelColumn++) {
                    nesByte paletteColorIndex = ((lineMsbpByte & 0x01) << 1) | (lineLsbpByte & 0x01);
                    lineMsbpByte >>= 1;
                    lineLsbpByte >>= 1;
                    olc::Pixel color = getPaletteColor(paletteIndex, paletteColorIndex);
                    patternTables[patternTableIndex].SetPixel(
                        tileColumn * 8 + (7 - pixelColumn),
                        tileRow * 8 + pixelRow,
                        color
                    );
                }
            }
        }
    }

    return &patternTables[patternTableIndex];
}

olc::Pixel* Ppu::getActivePalettesColors() {
    for (int i = 0; i < 32; i++) {
        activePalettesColors[i] = nesMainPalette[readViaPpuBus(0x3F00 + i, true)];
    }

    return activePalettesColors;
}

nesByte* Ppu::getNameTable(int nameTableIndex) {
    return nameTables[nameTableIndex];
}

nesByte Ppu::cpuRead(nesWord address, bool onlyRead) {
    nesByte data = 0x00;

    switch (address) {
        // PPUCTRL
        case 0x0000:
            // no reads from PPUCTRL
            break;

        // PPUMASK
        case 0x0001:
            // no reads from PPUMASK
            break;

        // PPUSTATUS
        case 0x0002:
            // Only the three most significant bits matter, but stale ppu bus
            // data technically exists in the five least significant bits.
            data = ppuStatusRegister.getAsByte() | (ppuDataBuffer & 0x1F);
            ppuStatusRegister.verticalBlank = 0;
            ppuAddressLatchUseLoByte = false;
            break;

        // OAMADDR
        case 0x0003:
            // no reads from OAMADDR
            break;

        // OAMDATA
        case 0x0004:
            // while the seconary oam is being cleared, attempting to read $2004 will return $FF
            if (cycle >= 1 && cycle <= 64) {
                data = 0xFF;
            } else {
                data = oamBytes[oamAddress];
            }
            break;

        // PPUSCROLL
        case 0x0005:
            // no reads from PPUSCROLL
            break;

        // PPUADDR
        case 0x0006:
            // no reads from PPUADDR
            break;

        // PPUDATA
        case 0x0007:
            nesWord currentPpuAddress = vRamAddress.getAsWord();
            data = ppuDataBuffer;
            ppuDataBuffer = readViaPpuBus(currentPpuAddress, onlyRead);

            // Palette data does not have a buffered delay, though it is put in the buffer.
            if (currentPpuAddress >= 0x3F00 && currentPpuAddress <= 0x3FFF) {
                data = ppuDataBuffer;
            }

            vRamAddress.increment(ppuCtrlRegister.incrementMode ? 0x0020 : 0x0001);
            break;
    }

    return data;
}

void Ppu::cpuWrite(nesWord address, nesByte data) {
    switch (address) {
        // PPUCTRL
        case 0x0000:
            ppuCtrlRegister.setData(data);
            tRamAddress.nameTableX = (ppuCtrlRegister.nameTableX) ? 0x01 : 0x00;
            tRamAddress.nameTableY = (ppuCtrlRegister.nameTableY) ? 0x01 : 0x00;
            break;

        // PPUMASK
        case 0x0001:
            ppuMaskRegister.setData(data);
            break;

        // PPUSTATUS
        case 0x0002:
            // no writes to PPUSTATUS
            break;

        // OAMADDR
        case 0x0003:
            oamAddress = data;
            break;

        // OAMDATA
        case 0x0004:
            oamBytes[oamAddress++] = data;
            break;

        // PPUSCROLL
        case 0x0005:
            if (ppuAddressLatchUseLoByte) {
                tRamAddress.fineY = data & 0x07;
                tRamAddress.coarseY = (data >> 3) & 0x1F;
            } else {
                tRamAddress.coarseX = (data >> 3) & 0x1F;
                fineX = data & 0x07;
            }
            ppuAddressLatchUseLoByte = !ppuAddressLatchUseLoByte;
            break;

        // PPUADDR
        case 0x0006:
            if (ppuAddressLatchUseLoByte) {
                tRamAddress.coarseY = (tRamAddress.coarseY & 0x18) | ((data >> 5) & 0x07);
                tRamAddress.coarseX = data & 0x1F;
                vRamAddress.setData(tRamAddress.getAsWord());
            } else {
                tRamAddress.fineY = (data >> 4) & 0x03;
                tRamAddress.nameTableY = (data >> 3) & 0x01;
                tRamAddress.nameTableX = (data >> 2) & 0x01;
                tRamAddress.coarseY = (tRamAddress.coarseY & 0x07) | ((data & 0x03) << 3);
            }
            ppuAddressLatchUseLoByte = !ppuAddressLatchUseLoByte;
            break;

        // PPUDATA
        case 0x0007:
            writeViaPpuBus(vRamAddress.getAsWord(), data);
            vRamAddress.increment(ppuCtrlRegister.incrementMode ? 0x0020 : 0x0001);
            break;
    }
}

nesByte Ppu::readViaPpuBus(nesWord address, bool onlyRead) {
    nesByte data = 0x00;

    address &= 0x3FFF;
    if (address >= 0x0000 && address <= 0x1FFF) {
        data = cartridge->ppuRead(address);
    } else if (address >= 0x2000 && address <= 0x3EFF) {
        address &= 0x0FFF;
        if (cartridge->getMirroring() == Cartridge::Mirroring::VERTICAL) {
            if ((address >= 0x0000 && address <= 0x03FF) || (address >= 0x0800 && address <= 0x0BFF)) {
                data = nameTables[0][address & 0x03FF];
            } else if ((address >= 0x0400 && address <= 0x07FF) || (address >= 0x0C00 && address <= 0x0FFF)) {
                data = nameTables[1][address & 0x03FF];
            }
        } else if (cartridge->getMirroring() == Cartridge::Mirroring::HORIZONTAL) {
            if ((address >= 0x0000 && address <= 0x03FF) || (address >= 0x0400 && address <= 0x07FF)) {
                data = nameTables[0][address & 0x03FF];
            } else if ((address >= 0x0800 && address <= 0x0BFF) || (address >= 0x0C00 && address <= 0x0FFF)) {
                data = nameTables[1][address & 0x03FF];
            }
        }
    } else if (address >= 0x3F00 && 0x3FFF) {
        address &= 0x001F;
        if (address == 0x0010) {
            address = 0x0000;
        } else if (address == 0x0014) {
            address = 0x0004;
        } else if (address == 0x0018) {
            address = 0x0008;
        } else if (address == 0x001C) {
            address = 0x000C;
        }

        data = activePalettesBytes[address];
    }

    return data;
}

void Ppu::writeViaPpuBus(nesWord address, nesByte data) {
    address &= 0x3FFF;
    if (address >= 0x0000 && address <= 0x1FFF) {
        cartridge->ppuWrite(address, data);
    } else if (address >= 0x2000 && address <= 0x3EFF) {
        address &= 0x0FFF;
        if (cartridge->getMirroring() == Cartridge::Mirroring::VERTICAL) {
            if ((address >= 0x0000 && address <= 0x03FF) || (address >= 0x0800 && address <= 0x0BFF)) {
                nameTables[0][address & 0x03FF] = data;
            } else if ((address >= 0x0400 && address <= 0x07FF) || (address >= 0x0C00 && address <= 0x0FFF)) {
                nameTables[1][address & 0x03FF] = data;
            }
        } else if (cartridge->getMirroring() == Cartridge::Mirroring::HORIZONTAL) {
            if ((address >= 0x0000 && address <= 0x03FF) || (address >= 0x0400 && address <= 0x07FF)) {
                nameTables[0][address & 0x03FF] = data;
            } else if ((address >= 0x0800 && address <= 0x0BFF) || (address >= 0x0C00 && address <= 0x0FFF)) {
                nameTables[1][address & 0x03FF] = data;
            }
        }
    } else if (address >= 0x3F00 && 0x3FFF) {
        address &= 0x001F;
        if (address == 0x0010) {
            address = 0x0000;
        } else if (address == 0x0014) {
            address = 0x0004;
        } else if (address == 0x0018) {
            address = 0x0008;
        } else if (address == 0x001C) {
            address = 0x000C;
        }

        activePalettesBytes[address] = data;
    }
}

void Ppu::clockTick() {
    if (scanline >= -1 && scanline <= 239) {
        if (scanline == -1) {
            if (cycle == 1) {
                ppuStatusRegister.verticalBlank = 0;
                ppuStatusRegister.spriteOverflow = 0;
                ppuStatusRegister.spriteZeroHit = 0;
            } else if (cycle >= 280 && cycle <= 304) {
                resetVerticalScroll();
            }
        }

        // === BACKGROUND ===

        if ((cycle >= 1 && cycle <= 256) || (cycle >= 321 && cycle <= 336)) {
            updateShiftRegisters();

            switch ((cycle - 1) % 8) {
                case 0:
                    transferBackgroundBytesToShiftRegisters();
                    break;

                case 1:
                    nextBackgroundNameTableByte = readViaPpuBus(0x2000 | (vRamAddress.getAsWord() & 0x0FFF));
                    break;

                case 3:
                    nextBackgroundAttributeByte = readViaPpuBus(
                        0x23C0 |
                        (vRamAddress.nameTableY << 11) |
                        (vRamAddress.nameTableX << 10) |
                        ((vRamAddress.coarseY >> 2) << 3) |
                        (vRamAddress.coarseX >> 2)
                    );

                    if (vRamAddress.coarseY & 0x02) {
                        nextBackgroundAttributeByte >>= 4;
                    }

                    if (vRamAddress.coarseX & 0x02) {
                        nextBackgroundAttributeByte >>= 2;
                    }

                    nextBackgroundAttributeByte &= 0x03;
                    break;

                case 5:
                    nextBackgroundLsbpByte = readViaPpuBus(
                        (ppuCtrlRegister.patternBackground ? 0x1000 : 0x0000) +
                        (((nesWord)nextBackgroundNameTableByte) << 4) +
                        vRamAddress.fineY
                    );
                    break;

                case 7:
                    nextBackgroundMsbpByte = readViaPpuBus(
                        (ppuCtrlRegister.patternBackground ? 0x1000 : 0x0000) +
                        (((nesWord)nextBackgroundNameTableByte) << 4) +
                        vRamAddress.fineY + 0x08
                    );

                    incrementHorizontalScroll();
                    break;
            }
        }

        if (cycle == 256) {
            incrementVerticalScroll();
        }

        if (cycle == 257) {
            resetHorizontalScroll();
        }

        if (cycle == 338 || cycle == 240) {
            nextBackgroundNameTableByte = readViaPpuBus(0x2000 | (vRamAddress.getAsWord() & 0x0FFF));
        }

        // === FOREGROUND ===

        // NOTE: We're going to just perform all the secondary oam clearing, sprite evaluation
        // logic, and data fetches on the last cycle to simplify things. In the future, we
        // should perform sprite evaluation and data fetches on a cycle-by-cycle basis to make
        // it more accurate.
        if (cycle == 256) {
            performSpriteEvaluation();
        } else if (cycle == 340) {
            loadSpriteShiftRegisters();
        }

        if (scanline >= 0 && scanline <= 239 && cycle >= 1 && cycle <= 256) {
            int backgroundPaletteIndex = 0;
            int backgroundPaletteColorIndex = 0;
            if (ppuMaskRegister.renderBackground && (ppuMaskRegister.renderBackgroundLeft || cycle > 8)) {
                nesWord bitMask = 0x8000 >> fineX;
                backgroundPaletteIndex =
                    (backgroundAttributeMsbShiftRegister & bitMask ? 0x02 : 0x00) |
                    (backgroundAttributeLsbShiftRegister & bitMask ? 0x01 : 0x00);
                backgroundPaletteColorIndex =
                    (backgroundPatternMsbShiftRegister & bitMask ? 0x02 : 0x00) |
                    (backgroundPatternLsbShiftRegister & bitMask ? 0x01 : 0x00);
            }

            spriteZeroIsBeingRendered = false;
            int spritePaletteIndex = 0;
            int spritePaletteColorIndex = 0;
            bool spriteHasRenderingPriority;
            if (ppuMaskRegister.renderSprites && (ppuMaskRegister.renderSpritesLeft || cycle > 8)) {
                for (int spriteIndex = 0; spriteIndex < 8; spriteIndex++) {
                    struct spriteEntry* sprite = &secondaryOam[spriteIndex];
                    if (sprite->x == 0) {
                        spritePaletteIndex = (sprite->attributes & 0x03) + 4;
                        spritePaletteColorIndex =
                            (spriteMsbShiftRegisters[spriteIndex] & 0x80 ? 0x02 : 0x00) |
                            (spriteLsbShiftRegisters[spriteIndex] & 0x80 ? 0x01 : 0x00);

                        spriteHasRenderingPriority = (sprite->attributes & 0x20) == 0;
                        if (spritePaletteColorIndex != 0x00) {
                            if (spriteIndex == 0) {
                                spriteZeroIsBeingRendered = true;
                            }
                            break;
                        }
                    }
                }
            }

            int paletteIndex = 0;
            int paletteColorIndex = 0;
            if (backgroundPaletteColorIndex == 0 && spritePaletteColorIndex != 0) {
                paletteIndex = spritePaletteIndex;
                paletteColorIndex = spritePaletteColorIndex;
            } else if (backgroundPaletteColorIndex != 0 && spritePaletteColorIndex == 0) {
                paletteIndex = backgroundPaletteIndex;
                paletteColorIndex = backgroundPaletteColorIndex;
            } else if (backgroundPaletteColorIndex != 0 && spritePaletteColorIndex != 0) {
                if (spriteHasRenderingPriority) {
                    paletteIndex = spritePaletteIndex;
                    paletteColorIndex = spritePaletteColorIndex;
                } else {
                    paletteIndex = backgroundPaletteIndex;
                    paletteColorIndex = backgroundPaletteColorIndex;
                }

                if (spriteZeroHitIsPossible && spriteZeroIsBeingRendered && ppuMaskRegister.renderBackground && ppuMaskRegister.renderSprites) {
                    int leftmostCycleToCheck = (~(ppuMaskRegister.renderBackgroundLeft | ppuMaskRegister.renderSpritesLeft)) ? 9 : 1;
                    if (cycle >= leftmostCycleToCheck && cycle <= 256) {
                        ppuStatusRegister.spriteZeroHit = 1;
                    }
                }
            }

            screen.SetPixel(cycle - 1, scanline, getPaletteColor(paletteIndex, paletteColorIndex));
        }
    }

    if (scanline == 241 && cycle == 1) {
        ppuStatusRegister.verticalBlank = 1;
        if (ppuCtrlRegister.nmiEnabled) {
            wasNmiSignaled = true;
        }
    }

    cycle++;
    if (cycle >= 341) {
        cycle = 0;

        scanline++;
        if (scanline >= 261) {
            scanline = -1;
            frameIsComplete = true;
        }
    }
}

void Ppu::incrementHorizontalScroll() {
    if (!ppuMaskRegister.renderBackground && !ppuMaskRegister.renderSprites) {
        return;
    }

    if (vRamAddress.coarseX == 31) {
        vRamAddress.coarseX = 0;
        vRamAddress.nameTableX = ~vRamAddress.nameTableX;
    } else {
        vRamAddress.coarseX++;
    }
}

void Ppu::incrementVerticalScroll() {
    if (!ppuMaskRegister.renderBackground && !ppuMaskRegister.renderSprites) {
        return;
    }

    if (vRamAddress.fineY == 7) {
        vRamAddress.fineY = 0;
        if (vRamAddress.coarseY == 29) {
            vRamAddress.coarseY = 0;
            vRamAddress.nameTableY = ~vRamAddress.nameTableY;
        } else {
            vRamAddress.coarseY++;
        }
    } else {
        vRamAddress.fineY++;
    }
}

void Ppu::resetHorizontalScroll() {
    if (!ppuMaskRegister.renderBackground && !ppuMaskRegister.renderSprites) {
        return;
    }

    vRamAddress.coarseX = tRamAddress.coarseX;
    vRamAddress.nameTableX = tRamAddress.nameTableX;
}

void Ppu::resetVerticalScroll() {
    if (!ppuMaskRegister.renderBackground && !ppuMaskRegister.renderSprites) {
        return;
    }

    vRamAddress.fineY = tRamAddress.fineY;
    vRamAddress.coarseY = tRamAddress.coarseY;
    vRamAddress.nameTableY = tRamAddress.nameTableY;
}

void Ppu::transferBackgroundBytesToShiftRegisters() {
    backgroundPatternLsbShiftRegister = (backgroundPatternLsbShiftRegister & 0xFF00) | nextBackgroundLsbpByte;
    backgroundPatternMsbShiftRegister = (backgroundPatternMsbShiftRegister & 0xFF00) | nextBackgroundMsbpByte;

    backgroundAttributeLsbShiftRegister = (backgroundAttributeLsbShiftRegister & 0xFF00) | (nextBackgroundAttributeByte & 0x01 ? 0xFF : 0x00);
    backgroundAttributeMsbShiftRegister = (backgroundAttributeMsbShiftRegister & 0xFF00) | (nextBackgroundAttributeByte & 0x02 ? 0xFF : 0x00);
}

void Ppu::updateShiftRegisters() {
    if (ppuMaskRegister.renderBackground) {
        backgroundPatternLsbShiftRegister <<= 1;
        backgroundPatternMsbShiftRegister <<= 1;

        backgroundAttributeLsbShiftRegister <<= 1;
        backgroundAttributeMsbShiftRegister <<= 1;
    }

    if (ppuMaskRegister.renderSprites && cycle >= 2 && cycle <= 257) {
        for (int spriteIndex = 0; spriteIndex < 8; spriteIndex++) {
            struct spriteEntry* sprite = &secondaryOam[spriteIndex];
            if (sprite->x > 0) {
                sprite->x--;
            } else {
                spriteLsbShiftRegisters[spriteIndex] <<= 1;
                spriteMsbShiftRegisters[spriteIndex] <<= 1;
            }
        }
    }
}

void Ppu::writeToOam(nesByte address, nesByte data) {
    oamBytes[address] = data;
}

nesByte* Ppu::getOamBytes() {
    return oamBytes;
}

void Ppu::performSpriteEvaluation() {
    for (int i = 0; i < 32; i++) {
        secondaryOamBytes[i] = 0xFF;
    }

    spriteZeroHitIsPossible = false;
    int spritesFoundOnScanline = 0;
    for (int spriteIndex = 0; spriteIndex < 64; spriteIndex++) {
        struct spriteEntry* sprite = &oam[spriteIndex];
        int distanceToSprite = scanline - sprite->y;
        if (distanceToSprite >= 0 && distanceToSprite < (ppuCtrlRegister.spriteSize ? 16 : 8)) {
            if (spritesFoundOnScanline < 8) {
                if (spriteIndex == 0) {
                    spriteZeroHitIsPossible = true;
                }

                secondaryOam[spritesFoundOnScanline++] = *sprite;
            } else {
                ppuStatusRegister.spriteOverflow = 1;
                break;
            }
        }
    }
}

void Ppu::loadSpriteShiftRegisters() {
    for (int spriteIndex = 0; spriteIndex < 8; spriteIndex++) {
        struct spriteEntry* sprite = &secondaryOam[spriteIndex];
        bool shouldFlipVertically = sprite->attributes & 0x80;
        bool shouldFlipHorizontally = sprite->attributes & 0x40;

        nesWord spriteLsbpByteAddress;
        nesWord spriteMsbpByteAddress;

        if (ppuCtrlRegister.spriteSize) {
            // 8 x 16 sprites
            if (sprite->attributes & 0x80) {
                if (scanline - sprite->y < 8) {
                    spriteLsbpByteAddress =
                        ((sprite->patternId & 0x01) << 12) |
                        (((sprite->patternId & 0xFE) + 1) << 4) |
                        (7 - (scanline - sprite->y) & 0x07)
                    ;
                } else {
                    spriteLsbpByteAddress =
                        ((sprite->patternId & 0x01) << 12) |
                        ((sprite->patternId & 0xFE) << 4) |
                        (7 - (scanline - sprite->y) & 0x07)
                    ;
                }
            } else {
                if (scanline - sprite->y < 8) {
                    spriteLsbpByteAddress =
                        ((sprite->patternId & 0x01) << 12) |
                        ((sprite->patternId & 0xFE) << 4) |
                        ((scanline - sprite->y) & 0x07)
                    ;
                } else {
                    spriteLsbpByteAddress =
                        ((sprite->patternId & 0x01) << 12) |
                        (((sprite->patternId & 0xFE) + 1) << 4) |
                        ((scanline - sprite->y) & 0x07)
                    ;
                }

            }
        } else {
            // 8 x 8 sprites
            nesWord row = scanline - sprite->y;
            if (shouldFlipVertically) {
                row = 7 - row;
            }

            spriteLsbpByteAddress =
                (ppuCtrlRegister.patternSprite << 12) |
                (sprite->patternId << 4) |
                row
            ;
        }

        spriteMsbpByteAddress = spriteLsbpByteAddress + 8;

        nesByte spriteLsbpByte = readViaPpuBus(spriteLsbpByteAddress);
        nesByte spriteMsbpByte = readViaPpuBus(spriteMsbpByteAddress);
        if (shouldFlipHorizontally) {
            // https://stackoverflow.com/a/2602885

            spriteLsbpByte = ((spriteLsbpByte & 0xF0) >> 4) | ((spriteLsbpByte & 0x0F) << 4);
            spriteLsbpByte = ((spriteLsbpByte & 0xCC) >> 2) | ((spriteLsbpByte & 0x33) << 2);
            spriteLsbpByte = ((spriteLsbpByte & 0xAA) >> 1) | ((spriteLsbpByte & 0x55) << 1);

            spriteMsbpByte = ((spriteMsbpByte & 0xF0) >> 4) | ((spriteMsbpByte & 0x0F) << 4);
            spriteMsbpByte = ((spriteMsbpByte & 0xCC) >> 2) | ((spriteMsbpByte & 0x33) << 2);
            spriteMsbpByte = ((spriteMsbpByte & 0xAA) >> 1) | ((spriteMsbpByte & 0x55) << 1);
        }

        spriteLsbShiftRegisters[spriteIndex] = spriteLsbpByte;
        spriteMsbShiftRegisters[spriteIndex] = spriteMsbpByte;
    }
}
