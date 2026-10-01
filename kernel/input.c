#include "io.h"

unsigned char get_scancode() {
    unsigned char status = inb(0x64);
    if (status & 0x01) {
        return inb(0x60);
    }
    return 0xFF; // нет нажатия
}
