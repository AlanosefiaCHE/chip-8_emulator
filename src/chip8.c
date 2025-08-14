#include <string.h>
#include <assert.h>
#include "chip8.h"
#include "chip8memory.h"

const char chip8_default_chracater_set[] = {
    0xf0, 0x90, 0x90, 0x90, 0xf0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xf0, 0x10, 0xf0, 0x80, 0xf0,
    0xf0, 0x10, 0xf0, 0x10, 0xf0,
    0x90, 0x90, 0xf0, 0x10, 0x10,
    0xf0, 0x80, 0xf0, 0x10, 0xf0,
    0xf0, 0x80, 0xf0, 0x90, 0xf0,
    0xf0, 0x10, 0x20, 0x40, 0x40,
    0xf0, 0x90, 0xf0, 0x90, 0xf0,
    0xf0, 0x90, 0xf0, 0x10, 0xf0,
    0xf0, 0x90, 0xf0, 0x90, 0x90,
    0xe0, 0x90, 0xe0, 0x90, 0xe0,
    0xf0, 0x80, 0x80, 0x80, 0xf0,
    0xe0, 0x90, 0x90, 0x90, 0xe0,
    0xf0, 0x80, 0xf0, 0x80, 0xf0, 
    0xf0, 0x80, 0xf0, 0x80, 0x80
};

void chip8_init(struct chip8 *chip8)
{
  memset(chip8, 0, sizeof(struct chip8)); // Zet de CHIP-8 struct op 0
  memcpy(&chip8->memory.memory, chip8_default_chracater_set, sizeof(chip8_default_chracater_set)); // De chip8_default_chracater_set wordt naar de juiste locatie gekopieerd in het geheugen
}
void chip8_load(struct chip8 *chip8, const char* buf, size_t size)
{
  assert(size+ CHIP8_PROGRAM_LOAD_ADDRESS < CHIP8_MEMORY_SIZE ); // Check of het programma binnen de geheugenlimieten past, wordt geplust omdat het programma op geheugen adres 0x200 wordt geladen.
  memcpy(&chip8->memory.memory[CHIP8_PROGRAM_LOAD_ADDRESS], buf, size); // Kopieert het programma naar de juiste locatie in het geheugen
  chip8->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS; // Zet de Program Counter naar het begin van het programma, zodat de juiste instructies worden gelezen
}
/// @brief Handle extended opcodes here
/// @param chip8 
/// @param opcode 
static void chip8_exec_extended(struct chip8* chip8, unsigned short opcode)
{
  unsigned short nnn = opcode & 0x0FFF; // nnn is de 12 bits die de adres aanduidt
  unsigned char x = (opcode >> 8) & 0x000F; 
  unsigned char kk = opcode & 0x00FF; 

  switch (opcode & 0xF000) //0f000 is een mask die zorgt dat alleen de eerste 4 bits worden bekeken
    {
     case 0x1000: // JP addr, 1nnn Jump to location nnns
    chip8 -> registers.PC = nnn; // Zet de Program Counter naar het adres
    break;

    case 0x2000: // Call addr, 2nnn call subroutine at location nnnn
    chip8_stack_push(chip8, chip8->registers.PC);
    chip8->registers.PC = nnn;
    break;

    case 0x3000: // SE Vx, byte, 3xkk Skip next instruction if Vx == kk
    if (chip8->registers.V[x] == kk)
      chip8->registers.PC += 2; //skips next instruction because instruction is 2 bytes
    break;

  }
}

void chip8_exec(struct chip8* chip8, unsigned short opcode)/// opcode is 2bytes groot, omdat de instructies ook 2bytes groot zijn
{
  // Chip-8 instructions based on the Chip-8 documentation 3.1: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
  switch (opcode)
  {
     case 0x00E0://CLS: clear the display
     chip8_screen_clear(&chip8->screen); 
    break;
   
    case 0X00EE: //RET: return from subroutine
    chip8->registers.PC = chip8_stack_pop(chip8);
    break;
  default:
    chip8_exec_extended(chip8, opcode);
  }
}
    