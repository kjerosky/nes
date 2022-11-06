#include "Ppu.h"

Ppu::Ppu(Cartridge* cartridge) {
    this->cartridge = cartridge;

    screen = olc::Sprite(256, 240);
    patternTables[0] = olc::Sprite(128, 128);
    patternTables[1] = olc::Sprite(128, 128);

    initializePalette();

    scanline = 0;
    cycle = 0;
    frameIsComplete = false;
    wasNmiSignaled = false;

    ppuCtrlRegister = 0x00;
    ppuMaskRegister = 0x00;
    ppuStatusRegister = 0x00;

    ppuAddressLatchUseLoByte = false;
    ppuDataBuffer = 0x00;
    ppuAddress = 0x0000;
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
            data = (ppuStatusRegister & 0xE0) | (ppuDataBuffer & 0x1F);
            ppuStatusRegister &= ~PPUSTATUS_VERTICAL_BLANK_BIT;
            ppuAddressLatchUseLoByte = false;
            break;

        // OAMADDR
        case 0x0003:
            // no reads from OAMADDR
            break;

        // OAMDATA
        case 0x0004:
            //TODO
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
            data = ppuDataBuffer;
            ppuDataBuffer = readViaPpuBus(ppuAddress, onlyRead);

            // Palette data does not have a buffered delay, though it is put in the buffer.
            if (ppuAddress >= 0x3F00 && ppuAddress <= 0x3FFF) {
                data = ppuDataBuffer;
            }

            ppuAddress += (ppuCtrlRegister & PPUCTRL_INCREMENT_MODE_BIT) ? 0x0020 : 0x0001;
            break;
    }

    return data;
}

void Ppu::cpuWrite(nesWord address, nesByte data) {
    switch (address) {
        // PPUCTRL
        case 0x0000:
            ppuCtrlRegister = data;
            break;

        // PPUMASK
        case 0x0001:
            ppuMaskRegister = data;
            break;

        // PPUSTATUS
        case 0x0002:
            // no writes to PPUSTATUS
            break;

        // OAMADDR
        case 0x0003:
            //TODO
            break;

        // OAMDATA
        case 0x0004:
            //TODO
            break;

        // PPUSCROLL
        case 0x0005:
            //TODO
            break;

        // PPUADDR
        case 0x0006:
            if (ppuAddressLatchUseLoByte) {
                ppuAddress = (ppuAddress & 0xFF00) | data;
            } else {
                ppuAddress = (ppuAddress & 0x00FF) | (data << 8);
            }
            ppuAddressLatchUseLoByte = !ppuAddressLatchUseLoByte;
            break;

        // PPUDATA
        case 0x0007:
            writeViaPpuBus(ppuAddress, data);
            ppuAddress += (ppuCtrlRegister & PPUCTRL_INCREMENT_MODE_BIT) ? 0x0020 : 0x0001;
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
    if (scanline == -1 && cycle == 1) {
        ppuStatusRegister &= ~PPUSTATUS_VERTICAL_BLANK_BIT;
    }

    if (scanline == 241 && cycle == 1) {
        ppuStatusRegister |= PPUSTATUS_VERTICAL_BLANK_BIT;
        if (ppuCtrlRegister & PPUCTRL_ENABLE_NMI_BIT) {
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
