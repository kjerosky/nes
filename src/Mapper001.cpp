#include "Mapper001.h"

// --------------------------------------------------------------------------

Mapper001::Mapper001(unsigned char programRomBankCount, unsigned char characterRomBankCount)
: Mapper(programRomBankCount, characterRomBankCount) {

    reset();
}

// --------------------------------------------------------------------------

Mapper001::~Mapper001() {
    // nothing to do for now
}

// --------------------------------------------------------------------------

void Mapper001::reset() {
    shift_register.reset();
    control_register.reset();
}

// --------------------------------------------------------------------------

bool Mapper001::mapCpuRead(Uint16 requestedAddress, unsigned int& mappedAddress) {
    // Handle 32kb banks
    if (control_register.prg_rom_bank_size == 0) {
        mappedAddress = (prg_bank & 0x0E) * 16384 + (requestedAddress & 0x7FFF);
        return true;
    }

    // Handle 16kb banks
    if (control_register.prg_rom_swap_bank == 0) {
        if (requestedAddress >= 0x8000 && requestedAddress <= 0xBFFF) {
            mappedAddress = requestedAddress & 0x3FFF;
        } else {
            mappedAddress = (prg_bank & 0x0F) * 16384 + (requestedAddress & 0x3FFF);
        }
    } else {
        if (requestedAddress >= 0x8000 && requestedAddress <= 0xBFFF) {
            mappedAddress = (prg_bank & 0x0F) * 16384 + (requestedAddress & 0x3FFF);
        } else {
            mappedAddress = (programRomBankCount - 1) * 16384 + (requestedAddress & 0x3FFF);
        }
    }

    return true;
}

// --------------------------------------------------------------------------

bool Mapper001::mapCpuWrite(Uint16 requestedAddress, unsigned int& mappedAddress, Uint8 data) {
    if ((data & 0x80) != 0) {
        reset();
        return false;
    }

    Uint8 ready_value;
    bool is_value_ready = shift_register.write_bit(data, ready_value);
    if (is_value_ready) {
        int target_register = (requestedAddress >> 13) & 0x03;
        switch (target_register) {
            case 0:
                control_register.set_values(ready_value);
                break;

            case 1:
                chr_bank_0 = ready_value;
                break;

            case 2:
                chr_bank_1 = ready_value;
                break;

            case 3:
                prg_bank = ready_value;
                break;
        }
    }

    return false;
}

// --------------------------------------------------------------------------

bool Mapper001::mapPpuRead(Uint16 requestedAddress, unsigned int& mappedAddress) {
    if (control_register.chr_rom_bank_size == 0) {
        // 8kb banks
        mappedAddress = (chr_bank_0 & 0x1E) * 4096 + (requestedAddress & 0x1FFF);
    } else {
        // 4kb banks
        if (requestedAddress >= 0x0000 && requestedAddress <= 0x0FFF) {
            mappedAddress = (chr_bank_0 & 0x1F) * 4096 + (requestedAddress & 0x0FFF);
        } else {
            mappedAddress = (chr_bank_1 & 0x1F) * 4096 + (requestedAddress & 0x0FFF);
        }
    }

    return true;
}

// --------------------------------------------------------------------------

bool Mapper001::mapPpuWrite(Uint16 requestedAddress, unsigned int& mappedAddress) {
    if (control_register.chr_rom_bank_size == 0) {
        // 8kb banks
        mappedAddress = (chr_bank_0 & 0x1E) * 4096 + (requestedAddress & 0x1FFF);
    } else {
        // 4kb banks
        if (requestedAddress >= 0x0000 && requestedAddress <= 0x0FFF) {
            mappedAddress = (chr_bank_0 & 0x1F) * 4096 + (requestedAddress & 0x0FFF);
        } else {
            mappedAddress = (chr_bank_1 & 0x1F) * 4096 + (requestedAddress & 0x0FFF);
        }
    }

    return true;
}

// --------------------------------------------------------------------------

bool Mapper001::handles_mirror(Mirroring& mirroring) {
    switch (control_register.nametable_arrangement) {
        case 0:
            mirroring = Mirroring::ONE_SCREEN_LOWER_BANK;
            break;

        case 1:
            mirroring = Mirroring::ONE_SCREEN_UPPER_BANK;
            break;

        case 2:
            mirroring = Mirroring::VERTICAL;
            break;

        case 3:
            mirroring = Mirroring::HORIZONTAL;
            break;
    }

    return true;
}
