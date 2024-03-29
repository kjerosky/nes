#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <string>
#include <vector>

#include "Types.h"
#include "Mapper.h"

class Cartridge {

public:

    Cartridge(std::string filename);
    ~Cartridge();

    bool isValid();

    void reset();

    nesByte cpuRead(nesWord address);
    void cpuWrite(nesWord address, nesByte data);
    nesByte ppuRead(nesWord address);
    void ppuWrite(nesWord address, nesByte data);

    enum Mirroring {
        HORIZONTAL,
        VERTICAL
    } mirroring;

    Mirroring getMirroring();

private:

    bool valid = false;

    std::vector<nesByte> programRomData;
    std::vector<nesByte> characterRomData;
    Mapper* mapper;
};

#endif
