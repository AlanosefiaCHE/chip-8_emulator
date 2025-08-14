#ifndef CHIP8_STACK_H
#define CHIP8_STACK_H

#include "config.h"
/*
 * chip8stack.h
 * Bevat de stack van de CHIP-8 emulator.
 * Deze stack is essentieel voor het beheren van de uitvoering van subroutines.
 */
struct chip8;
struct chip8_stack
{
    unsigned short stack[CHIP8_TOTAL_STACK_DEPTH];
    unsigned char SP; // Stack Pointer, wijst naar de bovenkant van de stack
};

void chip8_stack_push(struct chip8* chip8, unsigned short value);
unsigned short chip8_stack_pop(struct chip8* chip8);

#endif // CHIP8_STACK_H