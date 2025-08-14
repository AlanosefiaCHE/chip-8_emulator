#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H
#include  "config.h"

struct chip8_memory
{
   unsigned char memory[CHIP8_MEMORY_SIZE]; // geheugen die nodig is voor de CHIP- misschien veranderen naar unit8_t?
};

/// @brief Sets memory for chip8_memory array
/// @param index 
/// @param val 
void chip8_memory_set(struct chip8_memory *memory, int index, unsigned char val );

/// @brief Gets memory for chip8_memory array
/// @param index 
/// @return 
unsigned char chip8_memory_get(struct chip8_memory *memory, int index);

unsigned short chip8_memory_get_short(struct chip8_memory *memory, int index);
#endif // CHIP8_MEMORY_H