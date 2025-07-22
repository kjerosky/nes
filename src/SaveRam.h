#ifndef SAVE_RAM_H
#define SAVE_RAM_H

#include <SDL3/SDL.h>
#include <vector>

class SaveRam {

public:

    SaveRam();
    ~SaveRam();

    void reset();
    void clear();
    Uint8 read(Uint16 address);
    void write(Uint16 address, Uint8 value);
    bool load_from_disk();
    bool save_to_disk();
    void execute_periodic_actions();

private:

    static const Uint64 PERSIST_TO_DISK_WAIT_TIME_MS = 100;
    static const int CONTENTS_SIZE = 8192;

    std::vector<Uint8> contents;
    bool have_contents_changed;
    Uint64 contents_changed_timestamp;
};

#endif
