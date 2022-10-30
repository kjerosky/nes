#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <string>
#include <vector>

#include "Types.h"

class Cartridge {

public:

    Cartridge(std::string filename);
    ~Cartridge();

    bool isValid();

private:

    enum {
        HORIZONTAL,
        VERTICAL
    } mirroring;

    bool valid = false;

    std::vector<nesByte> programRomData;
    std::vector<nesByte> characterRomData;
};

#endif
