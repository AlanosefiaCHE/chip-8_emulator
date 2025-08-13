#include "chip8memory.h"
#include <assert.h>
  

/// @brief  Checks if index is above or equals to 0, or below (because the index starts at 0) the memory size
/// @param index 
static void chip8_memory_inbound_check(int index)
{
    assert(index >= 0 && index < CHIP8_MEMORY_SIZE);
}
/// @brief Check if memory is set inbound, and sets it
/// @param memory 
/// @param index 
/// @param val 
void chip8_memory_set(struct chip8_memory *memory, int index, unsigned char val) {
    chip8_memory_inbound_check(index);
    memory->memory[index] = val;
}
/// @brief Che
/// @param memory 
/// @param index 
/// @return 
unsigned char chip8_memory_get(struct chip8_memory *memory, int index) {
        return memory->memory[index];

}
