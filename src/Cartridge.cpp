#include "Cartridge.h"

#include <fstream>

#include "Mapper000.h"
#include "Mapper001.h"
#include "Mapper002.h"
#include "Mapper003.h"

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
        mirroring = (inesHeader.mapperData0 & 0x01) ? Mirroring::VERTICAL : Mirroring::HORIZONTAL;

        fprintf(stderr, "Loaded header for `%s':\n", filename.c_str());
        fprintf(stderr, "PRG ROM bank count: %u\n", programRomBankCount);
        fprintf(stderr, "CHR ROM bank count: %u\n", characterRomBankCount);
        fprintf(stderr, "Mapper id: %u\n", mapperId);
        fprintf(stderr, "Mirroring: %s\n", mirroring == Mirroring::VERTICAL ? "VERTICAL" : "HORIZONTAL");

        // skip trainer data for now, if it exists
        if (inesHeader.mapperData0 & 0x04) {
            inputFileStream.seekg(512, std::ios_base::cur);
        }

        programRomData.resize(16384 * programRomBankCount);
        inputFileStream.read((char*)programRomData.data(), programRomData.size());

        if (characterRomBankCount == 0) {
            // No CHR-ROM banks means that there is actually 8KB of CHR-RAM.
            characterRomData.resize(8192 * 1);
        } else {
            characterRomData.resize(8192 * characterRomBankCount);
            inputFileStream.read((char*)characterRomData.data(), characterRomData.size());
        }

        inputFileStream.close();

        switch (mapperId) {
            case 0:
                mapper = new Mapper000(programRomBankCount, characterRomBankCount);
                break;

            case 1:
                mapper = new Mapper001(programRomBankCount, characterRomBankCount);
                break;

            case 2:
                mapper = new Mapper002(programRomBankCount, characterRomBankCount);
                break;

            case 3:
                mapper = new Mapper003(programRomBankCount, characterRomBankCount);
                break;

            default:
                fprintf(stderr, "ERROR: Mapper %u has not been implemented yet!\n", mapperId);
                return;
        }

        valid = true;
    } else {
        fprintf(stderr, "ERROR: Could not open `%s'!\n", filename.c_str());
    }
}

Cartridge::~Cartridge() {
    delete mapper;
}

bool Cartridge::isValid() {
    return valid;
}

Mirroring Cartridge::getMirroring() {
    Mirroring override;
    if (mapper->handles_mirror(override)) {
        return override;
    }

    return mirroring;
}

void Cartridge::reset() {
    if (mapper != nullptr) {
        mapper->reset();
    }
}

Uint8 Cartridge::cpuRead(Uint16 address) {
    unsigned int mappedAddress;
    if (mapper->mapCpuRead(address, mappedAddress)) {
        return programRomData[mappedAddress];
    }

    return 0x00;
}

void Cartridge::cpuWrite(Uint16 address, Uint8 data) {
    unsigned int mappedAddress;
    if (mapper->mapCpuWrite(address, mappedAddress, data)) {
        programRomData[mappedAddress] = data;
    }
}

Uint8 Cartridge::ppuRead(Uint16 address) {
    unsigned int mappedAddress;
    if (mapper->mapPpuRead(address, mappedAddress)) {
        return characterRomData[mappedAddress];
    }

    return 0x00;
}

void Cartridge::ppuWrite(Uint16 address, Uint8 data) {
    unsigned int mappedAddress;
    if (mapper->mapPpuWrite(address, mappedAddress)) {
        characterRomData[mappedAddress] = data;
    }
}
