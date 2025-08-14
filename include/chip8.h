#ifndef CHIP8_H
#define CHIP8_H

#include "config.h"
#include "chip8memory.h"
#include "chip8registers.h"
#include "chip8stack.h"
#include "chip8keyboard.h"

struct chip8
{
    struct chip8_memory memory; // Geheugen voor de CHIP-8
    struct chip8_stack stack; // Stack voor subroutine management
    struct chip8_registers registers; // Registers voor de CHIP-8
    struct chip8_keyboard keyboard;
};

#endif 