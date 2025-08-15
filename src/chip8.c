#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
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
    0xf0, 0x80, 0xf0, 0x80, 0x80};

void chip8_init(struct chip8 *chip8)
{
  memset(chip8, 0, sizeof(struct chip8));                                                          // Zet de CHIP-8 struct op 0
  memcpy(&chip8->memory.memory, chip8_default_chracater_set, sizeof(chip8_default_chracater_set)); // De chip8_default_chracater_set wordt naar de juiste locatie gekopieerd in het geheugen
}
void chip8_load(struct chip8 *chip8, const char *buf, size_t size)
{
  assert(size + CHIP8_PROGRAM_LOAD_ADDRESS < CHIP8_MEMORY_SIZE);        // Check of het programma binnen de geheugenlimieten past, wordt geplust omdat het programma op geheugen adres 0x200 wordt geladen.
  memcpy(&chip8->memory.memory[CHIP8_PROGRAM_LOAD_ADDRESS], buf, size); // Kopieert het programma naar de juiste locatie in het geheugen
  chip8->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;                     // Zet de Program Counter naar het begin van het programma, zodat de juiste instructies worden gelezen
}

static void chip8_exec_opcode_eight(struct chip8 *chip8, unsigned short opcode)
{
  unsigned char x = (opcode >> 8) & 0x000F;
  unsigned char y = (opcode >> 4) & 0x000F;
  unsigned char final_fourbits = opcode & 0x000F; // geeft het laatste getal van de opcode voor opcde 8xyN
  unsigned short temp = 0;

  switch (final_fourbits)
  {
  case 0x00: // 8xy0: LD Vx, Vy. Vx =Vy
    chip8->registers.V[x] = chip8->registers.V[y];
    break;
  case 0x01: // 8xy1: OR Vx, Vy. Vx = Vx OR Vy
    chip8->registers.V[x] |= chip8->registers.V[y];
    break;
  case 0x02: // 8xy2: AND  Vx, Vy. Vx = Vx AND Vy
    chip8->registers.V[x] &= chip8->registers.V[y];
    break;
  case 0x03: // 8xy3: XOR Vx, Vy. Vx = Vx XOR Vy
    chip8->registers.V[x] ^= chip8->registers.V[y];
    break;

  case 0x04: // 8xy4: ADD Vx, Vy. Vx = Vx + Vy, set VF = carry
    temp = chip8->registers.V[x] + chip8->registers.V[y];
    chip8->registers.V[0x0F] = false;
    if (temp > 0xFF)
    {
      chip8->registers.V[0x0F] = true;
    }
    chip8->registers.V[x] = temp;
    break;

  case 0x05: // 8xy5: SUB Vx, Vy. Vx = Vx - Vy, set VF = NOT borrow
    chip8->registers.V[0x0F] = false;
    if (chip8->registers.V[x] > chip8->registers.V[y]) // Check of er een borrow is
    {
      chip8->registers.V[0x0F] = true; // Er is geen borrow
    }
    chip8->registers.V[x] -= chip8->registers.V[y];
    break;

  case 0x06:                                                 // 8xy6: SHR Vx {, Vy}
    chip8->registers.V[0x0F] = chip8->registers.V[x] & 0x01; // Zet de carry flag naar de laatste bit van Vx
    chip8->registers.V[x] = chip8->registers.V[x] / 2;
    break;
  case 0x07:                                                                  // SUBN Vx, Vy
    chip8->registers.V[0x0F] = chip8->registers.V[y] > chip8->registers.V[x]; // one line if-statement
    chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];    // Vx = Vy - Vx
    break;
  case 0x0E:                                                       // 8xyE: SHL Vx {, Vy}
    chip8->registers.V[0x0F] = chip8->registers.V[x] & 0b10000000; // Zet de carry flag naar de eerste bit van Vx
    chip8->registers.V[x] = chip8->registers.V[x] * 2;
    break;
  default:
    break;
  }
}

static void chip8_exec_opcode_f(struct chip8 *chip8, unsigned short opcode)
{

}

/// @brief Handle extended opcodes here
/// @param chip8
/// @param opcode
static void chip8_exec_extended(struct chip8 *chip8, unsigned short opcode)
{
  unsigned short nnn = opcode & 0x0FFF;     // nnn is de 12 bits die de adres aanduidt
  unsigned char x = (opcode >> 8) & 0x000F; // Om x waarde uit opcode te krijgen
  unsigned char y = (opcode >> 4) & 0x000F; // Om y waarde uit opcode te krijgen
  unsigned char kk = opcode & 0x00FF;
  unsigned char n = opcode & 0x000F; // Height of the sprite

  switch (opcode & 0xF000) // 0f000 is een mask die zorgt dat alleen de eerste 4 bits worden bekeken
  {
  case 0x1000:                 // JP addr, 1nnn Jump to location nnns
    chip8->registers.PC = nnn; // Zet de Program Counter naar het adres
    break;

  case 0x2000: // Call addr, 2nnn call subroutine at location nnnn
    chip8_stack_push(chip8, chip8->registers.PC);
    chip8->registers.PC = nnn;
    break;

  case 0x3000: // SE Vx, byte, 3xkk Skip next instruction if Vx == kk
    if (chip8->registers.V[x] == kk)
      chip8->registers.PC += 2; // skips next instruction because instruction is 2 bytes
    break;

  case 0x4000: // SNE Vx, byte, 3xkk Skip next instruction if Vx != kk
    if (chip8->registers.V[x] != kk)
      chip8->registers.PC += 2;
    break;

  case 0x5000: // 5xy0: SE Vx, Vy, 5xy0 Skip next instruction if Vx == Vy
    if (chip8->registers.V[x] == chip8->registers.V[y])
      chip8->registers.PC += 2;
    break;

  case 0x6000: // 6xkk: LD Vx, byte, 6xkk Load Vx with kk
    chip8->registers.V[x] = kk;
    break;

  case 0x7000: // 7xkk: ADD Vx, byte, Set Vx = Vx + kk
    chip8->registers.V[x] += kk;
    break;

  case 0x8000:
    chip8_exec_opcode_eight(chip8, opcode);
    break;

  case 0x9000: // SNE Vx, Vy
    if (chip8->registers.V[x] != chip8->registers.V[y])
      chip8->registers.PC += 2;
    break;

  case 0xA000: // ANNN: LD I, addr
    chip8->registers.I = nnn;
    break;

  case 0xB000: // BNNN: JP V0, addr
    chip8->registers.PC = chip8->registers.V[0x00] + nnn;
    break;

  case 0xC000: // CXKK: RND Vx, byte
    srand(clock());
    chip8->registers.V[x] = (rand() % 255) & kk;
    break;
  case 0xD000: //Dxyn DRW Vx Vy nibble draw sprite to the screen
  {
    const char* sprite = (const char*)&chip8->memory + chip8->registers.I;
    chip8->registers.V[0x0f]= chip8_screen_draw_sprite(&chip8 -> screen, chip8->registers.V[x], chip8->registers.V[y], sprite, n);
  }
  break;

  case 0xE000:
  {
    switch(opcode & 0x00ff)
    {
          
      case 0x9e://EX9E - SKP Vx, skip the next instruction if the key with the value of Vx is pressed
      if(chip8_is_key_pressed(&chip8->keyboard, chip8->registers.V[x]))
      {
        chip8->registers.PC += 2; 
      }
      break;

      case 0xA1: //EXA1 - SKNP Vx, skip the next instruction if the key with the value of Vx is not pressed
      {
        if(!chip8_is_key_pressed(&chip8->keyboard, chip8->registers.V[x]))
          chip8->registers.PC += 2; 
      }
      break;
    }

  }
  break;
  case 0xF00:
  chip8_exec_opcode_f(chip8, opcode);
    break;
  }
}

void chip8_exec(struct chip8 *chip8, unsigned short opcode) /// opcode is 2bytes groot, omdat de instructies ook 2bytes groot zijn
{
  // Chip-8 instructions based on the Chip-8 documentation 3.1: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
  switch (opcode)
  {
  case 0x00E0: // CLS: clear the display
    chip8_screen_clear(&chip8->screen);
    break;

  case 0X00EE: // RET: return from subroutine
    chip8->registers.PC = chip8_stack_pop(chip8);
    break;
  default:
    chip8_exec_extended(chip8, opcode);
  }
}
