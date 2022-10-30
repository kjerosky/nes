#include "Cartridge.h"

#include <fstream>

Cartridge::Cartridge(std::string filename) {
    struct inesHeader {
        char nesIdentifier[4];
        unsigned char programRomBankCount;
        unsigned char characterRomBankCount;
        unsigned char mapperData0;
        unsigned char mapperData1;
        unsigned char programRamSize;
        unsigned char tvSystemData0;
        unsigned char tvSystemData1;
        unsigned char padding[5];
    } inesHeader;

    valid = false;

    std::ifstream inputFileStream;
    inputFileStream.open(filename, std::ifstream::binary);
    if (inputFileStream.is_open()) {
        inputFileStream.read((char*)(&inesHeader), sizeof(inesHeader));

        unsigned char programRomBankCount = inesHeader.programRomBankCount;
        unsigned char characterRomBankCount = inesHeader.characterRomBankCount;
        unsigned char mapperId = (inesHeader.mapperData1 & 0xF0) | (inesHeader.mapperData0 >> 4);
        mirroring = (inesHeader.mapperData0 & 0x01) ? VERTICAL : HORIZONTAL;

        fprintf(stderr, "Loaded header for `%s':\n", filename.c_str());
        fprintf(stderr, "PRG ROM bank count: %u\n", programRomBankCount);
        fprintf(stderr, "CHR ROM bank count: %u\n", characterRomBankCount);
        fprintf(stderr, "Mapper id: %u\n", mapperId);
        fprintf(stderr, "Mirroring: %s\n", mirroring == VERTICAL ? "VERTICAL" : "HORIZONTAL");

        // skip trainer data for now, if it exists
        if (inesHeader.mapperData0 & 0x04) {
            inputFileStream.seekg(512, std::ios_base::cur);
        }

        programRomData.resize(16384 * programRomBankCount);
        inputFileStream.read((char*)programRomData.data(), programRomData.size());

        characterRomData.resize(8192 * characterRomBankCount);
        inputFileStream.read((char*)characterRomData.data(), characterRomData.size());

        inputFileStream.close();

        valid = true;
    } else {
        fprintf(stderr, "ERROR: Could not open `%s'!\n", filename.c_str());
    }
}

Cartridge::~Cartridge() {
    // do nothing
}

bool Cartridge::isValid() {
    return valid;
}
