#ifndef CHIP8_KEYBOARD_H
#define CHIP8_KEYBOARD_H

#include <stdbool.h>
#include "config.h"
struct chip8_keyboard
{
    bool keyboard[CHIP8_TOTAL_KEYS]; // CHIP-8 heeft 16 toetsen
    const char* keyboard_map;
};
// the int key is the virtual key, not the desktop key
void chip8_keyboard_set_map(struct chip8_keyboard *keyboard, const char* map);
int chip8_keyboard_map(struct chip8_keyboard *keyboard, char key);
void chip8_pressed_key(struct chip8_keyboard *keyboard, int key);
void chip8_released_key(struct chip8_keyboard *keyboard, int key);
bool chip8_is_key_pressed(struct chip8_keyboard *keyboard, int key);

#endif 