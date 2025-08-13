#ifndef CHIP8_REGISTERS_H
#define CHIP8_REGISTERS_H
/*
 * chip8registers.h
 * Bevat de registers van de CHIP-8 emulator.
 * Deze registers zijn essentieel voor het uitvoeren van CHIP-8 instructies.
 */
#include "config.h"

struct chip8_registers
{
    unsigned char V[CHIP8_REGISTER_COUNT];
    unsigned short I;
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short PC;
    unsigned char SP;
};

#endif 