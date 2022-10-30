#include "Ppu.h"

Ppu::Ppu(Cartridge* cartridge) {
    this->cartridge = cartridge;

    screen = olc::Sprite(256, 240);

    initializePalette();

    scanline = 0;
    cycle = 0;
    frameIsComplete = false;
}

Ppu::~Ppu() {
    // do nothing
}

olc::Sprite* Ppu::getScreen() {
    return &screen;
}

void Ppu::initializePalette() {
    palette[0x00] = olc::Pixel(84, 84, 84);
    palette[0x01] = olc::Pixel(0, 30, 116);
    palette[0x02] = olc::Pixel(8, 16, 144);
    palette[0x03] = olc::Pixel(48, 0, 136);
    palette[0x04] = olc::Pixel(68, 0, 100);
    palette[0x05] = olc::Pixel(92, 0, 48);
    palette[0x06] = olc::Pixel(84, 4, 0);
    palette[0x07] = olc::Pixel(60, 24, 0);
    palette[0x08] = olc::Pixel(32, 42, 0);
    palette[0x09] = olc::Pixel(8, 58, 0);
    palette[0x0A] = olc::Pixel(0, 64, 0);
    palette[0x0B] = olc::Pixel(0, 60, 0);
    palette[0x0C] = olc::Pixel(0, 50, 60);
    palette[0x0D] = olc::Pixel(0, 0, 0);
    palette[0x0E] = olc::Pixel(0, 0, 0);
    palette[0x0F] = olc::Pixel(0, 0, 0);

    palette[0x10] = olc::Pixel(152, 150, 152);
    palette[0x11] = olc::Pixel(8, 76, 196);
    palette[0x12] = olc::Pixel(48, 50, 236);
    palette[0x13] = olc::Pixel(92, 30, 228);
    palette[0x14] = olc::Pixel(136, 20, 176);
    palette[0x15] = olc::Pixel(160, 20, 100);
    palette[0x16] = olc::Pixel(152, 34, 32);
    palette[0x17] = olc::Pixel(120, 60, 0);
    palette[0x18] = olc::Pixel(84, 90, 0);
    palette[0x19] = olc::Pixel(40, 114, 0);
    palette[0x1A] = olc::Pixel(8, 124, 0);
    palette[0x1B] = olc::Pixel(0, 118, 40);
    palette[0x1C] = olc::Pixel(0, 102, 120);
    palette[0x1D] = olc::Pixel(0, 0, 0);
    palette[0x1E] = olc::Pixel(0, 0, 0);
    palette[0x1F] = olc::Pixel(0, 0, 0);

    palette[0x20] = olc::Pixel(236, 238, 236);
    palette[0x21] = olc::Pixel(76, 154, 236);
    palette[0x22] = olc::Pixel(120, 124, 236);
    palette[0x23] = olc::Pixel(176, 98, 236);
    palette[0x24] = olc::Pixel(228, 84, 236);
    palette[0x25] = olc::Pixel(236, 88, 180);
    palette[0x26] = olc::Pixel(236, 106, 100);
    palette[0x27] = olc::Pixel(212, 136, 32);
    palette[0x28] = olc::Pixel(160, 170, 0);
    palette[0x29] = olc::Pixel(116, 196, 0);
    palette[0x2A] = olc::Pixel(76, 208, 32);
    palette[0x2B] = olc::Pixel(56, 204, 108);
    palette[0x2C] = olc::Pixel(56, 180, 204);
    palette[0x2D] = olc::Pixel(60, 60, 60);
    palette[0x2E] = olc::Pixel(0, 0, 0);
    palette[0x2F] = olc::Pixel(0, 0, 0);

    palette[0x30] = olc::Pixel(236, 238, 236);
    palette[0x31] = olc::Pixel(168, 204, 236);
    palette[0x32] = olc::Pixel(188, 188, 236);
    palette[0x33] = olc::Pixel(212, 178, 236);
    palette[0x34] = olc::Pixel(236, 174, 236);
    palette[0x35] = olc::Pixel(236, 174, 212);
    palette[0x36] = olc::Pixel(236, 180, 176);
    palette[0x37] = olc::Pixel(228, 196, 144);
    palette[0x38] = olc::Pixel(204, 210, 120);
    palette[0x39] = olc::Pixel(180, 222, 120);
    palette[0x3A] = olc::Pixel(168, 226, 144);
    palette[0x3B] = olc::Pixel(152, 226, 180);
    palette[0x3C] = olc::Pixel(160, 214, 228);
    palette[0x3D] = olc::Pixel(160, 162, 160);
    palette[0x3E] = olc::Pixel(0, 0, 0);
    palette[0x3F] = olc::Pixel(0, 0, 0);
}

bool Ppu::isFrameComplete() {
    return frameIsComplete;
}

void Ppu::acknowledgeFrameWasCompleted() {
    frameIsComplete = false;
}

nesWord Ppu::cpuRead(nesWord address, bool onlyRead) {
    //TODO
    return 0x00;
}

void Ppu::cpuWrite(nesWord address, nesByte data) {
    //TODO
}

void Ppu::clockTick() {
    // TODO REPLACE THIS RANDOM NOISE WITH ACTUAL DISPLAY
    screen.SetPixel(cycle - 1, scanline, palette[rand() % 2 ? 0x30 : 0x3F]);

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
