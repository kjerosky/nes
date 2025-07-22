#ifndef MAPPER001_H
#define MAPPER001_H

#include <SDL3/SDL.h>

#include "Mapper.h"

class Mapper001 : public Mapper {

public:

    Mapper001(unsigned char programRomBankCount, unsigned char characterRomBankCount);
    ~Mapper001();

    void reset() override;

    bool mapCpuRead(Uint16 requestedAddress, unsigned int& mappedAddress) override;
    bool mapCpuWrite(Uint16 requestedAddress, unsigned int& mappedAddress, Uint8 data) override;
    bool mapPpuRead(Uint16 requestedAddress, unsigned int& mappedAddress) override;
    bool mapPpuWrite(Uint16 requestedAddress, unsigned int& mappedAddress) override;
    bool handles_mirror(Mirroring& mirroring) override;

private:

    struct ShiftRegister {
        Uint8 contents;

        void reset() {
            contents = 0x10;
        }

        bool write_bit(Uint8 bit, Uint8& ready_value) {
            bool is_value_ready = (contents & 0x01) == 1;
            contents = (contents >> 1) | ((bit & 0x01) << 4);

            if (is_value_ready) {
                ready_value = contents;
                reset();
            }

            return is_value_ready;
        }
    } shift_register;

    struct ControlRegister {
        Uint8 nametable_arrangement;
        Uint8 prg_rom_swap_bank;
        Uint8 prg_rom_bank_size;
        Uint8 chr_rom_bank_size;

        void reset() {
            set_values(0x0C);
        }

        void set_values(Uint8 values_byte) {
            nametable_arrangement = values_byte & 0x03;
            prg_rom_swap_bank = (values_byte >> 2) & 0x01;
            prg_rom_bank_size = (values_byte >> 3) & 0x01;
            chr_rom_bank_size = (values_byte >> 4) & 0x01;
        }
    } control_register;

    Uint8 chr_bank_0;
    Uint8 chr_bank_1;
    Uint8 prg_bank;
};

#endif
