#include "SaveRam.h"

#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

extern std::string sram_file_name;

// --------------------------------------------------------------------------

SaveRam::SaveRam() {
    contents.resize(CONTENTS_SIZE);
    reset();
}

// --------------------------------------------------------------------------

SaveRam::~SaveRam() {
    // nothing to do for now
}

// --------------------------------------------------------------------------

void SaveRam::reset() {
    clear();

    std::filesystem::path sram_path(sram_file_name);
    if (std::filesystem::exists(sram_path)) {
        if (!load_from_disk()) {
            clear();
        }
    } else {
        save_to_disk();
    }

    have_contents_changed = false;
    contents_changed_timestamp = SDL_GetTicks();
}

// --------------------------------------------------------------------------

void SaveRam::clear() {
    for (int i = 0; i < contents.size(); i++) {
        contents[i] = 0;
    }
}

// --------------------------------------------------------------------------

Uint8 SaveRam::read(Uint16 address) {
    return contents[address & 0x1FFF];
}

// --------------------------------------------------------------------------

void SaveRam::write(Uint16 address, Uint8 value) {
    contents[address & 0x1FFF] = value;

    have_contents_changed = true;
    contents_changed_timestamp = SDL_GetTicks();
}

// --------------------------------------------------------------------------

bool SaveRam::load_from_disk() {
    std::ifstream file(sram_file_name, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "[ERROR] Could not open file to load sram contents: " << sram_file_name << std::endl;
        return false;
    }

    std::streamsize size = file.tellg();
    if (size != CONTENTS_SIZE) {
        std::cerr << "[ERROR] sram file (" << sram_file_name << ") is not of size " << CONTENTS_SIZE << ": " << size << std::endl;
        return false;
    }

    file.seekg(0, std::ios::beg);
    if (!file.read(reinterpret_cast<char*>(contents.data()), size)) {
        std::cerr << "[ERROR] Could not read from file: " << sram_file_name << std::endl;
        return false;
    }

    return true;
}

// --------------------------------------------------------------------------

bool SaveRam::save_to_disk() {
    std::ofstream file(sram_file_name, std::ios::binary);
    if (!file) {
        std::cerr << "[ERROR] Could not open file to save sram contents: " << sram_file_name << std::endl;
        return false;
    }

    file.write(reinterpret_cast<const char*>(contents.data()), contents.size());
    return file.good();
}

// --------------------------------------------------------------------------

void SaveRam::execute_periodic_actions() {
    if (!have_contents_changed) {
        return;
    }

    Uint64 ms_since_contents_changed = SDL_GetTicks() - contents_changed_timestamp;
    if (ms_since_contents_changed >= PERSIST_TO_DISK_WAIT_TIME_MS) {
        have_contents_changed = false;

        save_to_disk();
    }
}
