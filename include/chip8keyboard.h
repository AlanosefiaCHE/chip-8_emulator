#ifndef CHIP8_KEYBOARD_H
#define CHIP8_KEYBOARD_H

#include "config.h"

struct chip8_keyboard
{
    unsigned char key[16]; // CHIP-8 heeft 16 toetsen
};

#endif // CHIP8_KEYBOARD_H