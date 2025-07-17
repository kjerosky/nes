#include "TextUtils.h"

std::string hex(unsigned int number, int width) {
    std::string s(width, '0');
    for (int i = width - 1; i >= 0; i--, number >>= 4) {
        s[i] = "0123456789ABCDEF"[number & 0xF];
    }

    return s;
}
