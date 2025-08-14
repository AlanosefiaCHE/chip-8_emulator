#ifndef CONFIG_H
#define CONFIG_H
/* 
 Bevat de originele configuratie-instellingen voor de CHIP-8 emulator:
 http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.1
*/
#define CHIP8_MEMORY_SIZE 4096
#define CHIP8_WIDTH 64 
#define CHIP8_HEIGHT 32
// Schaalfactor voor het venster, omdat de chip8 display 64x32 is
#define CHIP8_WINDOW_MULTIPLIER 10

#define EMULATOR_WINDOW_TITLE "CHIP-8 Emulator"

#define CHIP8_REGISTER_COUNT 16
#define CHIP8_TOTAL_STACK_DEPTH 16

#define CHIP8_TOTAL_KEYS 16
#define CHIP8_CHARACTER_SET_LOAD_ADDRESS 0x00

#endif // CONFIG_H