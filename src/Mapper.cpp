#include "Mapper.h"

Mapper::Mapper(unsigned char programRomBankCount, unsigned char characterRomBankCount) {
    this->programRomBankCount = programRomBankCount;
    this->characterRomBankCount = characterRomBankCount;
}

Mapper::~Mapper() {
    // do nothing
}

void Mapper::reset() {
    // do nothing by default
}
