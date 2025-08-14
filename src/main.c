#include <SDL2/SDL.h>
#include <stdio.h>
#include "chip8.h"

int main(int argc, char* argv[]) {
    struct chip8 chip8;

    chip8.registers.SP = 0; // Initialiseer de stack pointer

    chip8_stack_push(&chip8, 0xff);
    chip8_stack_push(&chip8, 0xaa);

    printf("%x\n", chip8_stack_pop(&chip8));
    printf("%x\n", chip8_stack_pop(&chip8));

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        CHIP8_WIDTH * CHIP8_WINDOW_MULTIPLIER, CHIP8_HEIGHT * CHIP8_WINDOW_MULTIPLIER, SDL_WINDOW_SHOWN);

    printf("CHIP-8 Emulator started.\n");

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    while (1)
    {
        SDL_Event event;
        while(SDL_PollEvent(&event)) // zorgt ervoor dat events bv window bewegen afgehandeld kunnen worden
        {
            if(event.type == SDL_QUIT)
            {
              goto out;
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