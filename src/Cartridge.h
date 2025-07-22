#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <string>
#include <vector>
#include <SDL3/SDL.h>

#include "Mapper.h"
#include "Mirroring.h"

class Cartridge {

public:

    Cartridge(std::string filename);
    ~Cartridge();

    bool isValid();

    void reset();

    Uint8 cpuRead(Uint16 address);
    void cpuWrite(Uint16 address, Uint8 data);
    Uint8 ppuRead(Uint16 address);
    void ppuWrite(Uint16 address, Uint8 data);

    Mirroring mirroring;

    Mirroring getMirroring();

private:

    bool valid = false;

    std::vector<Uint8> programRomData;
    std::vector<Uint8> characterRomData;
    Mapper* mapper;
};

#endif
