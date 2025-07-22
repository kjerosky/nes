#ifndef SAVE_RAM_H
#define SAVE_RAM_H

#include <SDL3/SDL.h>

class SaveRam {

public:

    SaveRam();
    ~SaveRam();

    void clear();
    Uint8 read(Uint16 address);
    void write(Uint16 address, Uint8 value);

private:

    static const int CONTENTS_SIZE = 8192;
    Uint8 contents[CONTENTS_SIZE];
};

#endif
