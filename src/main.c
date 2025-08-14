#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include "chip8.h"
#include "chip8keyboard.h"

//Key board events worden door SDL, deze returnen code die gelijk staan aan de keyboard_map codes
const char keyboard_map[CHIP8_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3,
    SDLK_4, SDLK_5, SDLK_6, SDLK_7,
    SDLK_8, SDLK_9, SDLK_a, SDLK_b,
    SDLK_c, SDLK_d, SDLK_e, SDLK_f
};

int main(int argc, char* argv[]) {
    printf("CHIP-8 Emulator started.\n");

    struct chip8 chip8;
    printf("%x\n", chip8_keyboard_map(keyboard_map, 0xA));

    chip8_pressed_key(&chip8.keyboard, 0x0f);
    bool is_down = chip8_is_key_pressed(&chip8.keyboard, 0x0f);
    printf("Key 0x0f is %s\n", is_down ? "pressed" : "released");
    printf("Key 0x0f is %i\n", (int)is_down);

    chip8_released_key(&chip8.keyboard, 0x0f);
    is_down = chip8_is_key_pressed(&chip8.keyboard, 0x0f);
    printf("Key 0x0f is %s\n", is_down ? "pressed" : "released");
    printf("Key 0x0f is %i\n", (int)is_down);

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        CHIP8_WIDTH * CHIP8_WINDOW_MULTIPLIER, CHIP8_HEIGHT * CHIP8_WINDOW_MULTIPLIER, SDL_WINDOW_SHOWN);


    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    while (1)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event)) // zorgt ervoor dat events bv window bewegen afgehandeld kunnen worden
        {
            switch (event.type)
            {
            case SDL_QUIT:
                goto out;
                break;
            
            case SDL_KEYDOWN:
            {
                char key = event.key.keysym.sym;
                int virtual_key =chip8_keyboard_map(keyboard_map, key); // mapt event key naar virtual key
                if(virtual_key != -1)
                {
                    chip8_pressed_key(&chip8.keyboard, virtual_key);
                    printf("key is down %x\n", virtual_key);
                }

            }
                break;

            case SDL_KEYUP:
            {
                char key = event.key.keysym.sym;
                int virtual_key =chip8_keyboard_map(keyboard_map, key);
                if(virtual_key != -1)
                {
                    chip8_released_key(&chip8.keyboard, virtual_key);
                    printf("key is up %x\n", virtual_key);
                }

            }
                break;
            }
                     
        }
        SDL_SetRenderDrawColor(renderer,0,0,0,0); // Maakt de achtergrond zwart dmv RGB
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer,255,255,255,0); // Kleurt de rechthoek wit
        SDL_Rect rect = { 0, 0, 40, 40 };
        SDL_RenderFillRect(renderer, &rect); // Teken een witte rechthoek
        SDL_RenderPresent(renderer);
    }
    
out:
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}