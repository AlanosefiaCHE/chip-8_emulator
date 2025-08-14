#include "chip8.h"
#include "chip8stack.h"
#include <assert.h>
#include <stdio.h>

/// @brief Checks if the stack pointer is within bounds to prevent stack overflow
/// @param chip8 
static void chip8_stack_in_bounds(struct chip8* chip8)
{
    printf("Stack Pointer: %d\n", chip8->registers.SP);
      assert(chip8->registers.SP < CHIP8_TOTAL_STACK_DEPTH);
}
/// @brief Pushes a value onto the stack.
/// @param chip8 
/// @param value 
void chip8_stack_push(struct chip8* chip8, unsigned short value)
{
    chip8_stack_in_bounds(chip8);
    chip8->stack.stack[chip8->registers.SP] = value; // schrijft waarde op de stack
    chip8->registers.SP += 1; // SP naar de volgende vrije plek op de stack
}

/// @brief Pop a value from the stack
/// @param chip8 
/// @return 
unsigned short chip8_stack_pop(struct chip8* chip8)
{
    chip8 -> registers.SP -= 1;
    chip8_stack_in_bounds(chip8);
    unsigned short value  = chip8->stack.stack[chip8->registers.SP];
    return value;
}