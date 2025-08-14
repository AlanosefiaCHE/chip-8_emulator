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

void chip8_exec(struct chip8* chip8, unsigned short opcode)/// opcode is 2bytes groot, omdat de instructies ook 2bytes groot zijn
{

}
    