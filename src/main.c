#include <SDL2/SDL.h>
#include <stdio.h>
#include <Windows.h>
#include <stdbool.h>
#include "chip8.h"
#include "chip8keyboard.h"

// Key board events worden door SDL, deze returnen code die gelijk staan aan de keyboard_map codes
const char keyboard_map[CHIP8_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3,
    SDLK_4, SDLK_5, SDLK_6, SDLK_7,
    SDLK_8, SDLK_9, SDLK_a, SDLK_b,
    SDLK_c, SDLK_d, SDLK_e, SDLK_f};

int main(int argc, char *argv[])
{
    printf("CHIP-8 Emulator started.\n");

    if (argc < 2)
    {
        printf("You must provide a file to load\n");
        return -1;
    }

    const char* filename = argv[1];
    printf("Loading file: %s\n", filename);

    FILE* f = fopen(filename, "rb");
    if (!f)
    {
        printf("Failed to open file: %s\n", filename);
        return -1;
    }
    fseek(f, 0, SEEK_END); // Zet de bestandspositie naar het einde
    long size = ftell(f); // Verkrijg de grootte van het bestand
    fseek(f, 0, SEEK_SET); // Zet de bestandspositie terug naar het begin

    char buf[size];
    int res = fread(buf, size,1, f); // Lees de inhoud van het bestand in de buffer
    if (res !=1)
    {
        printf("Failed to read file: %s\n", filename);
        fclose(f);
        return -1;
    }

    struct chip8 chip8;
    chip8_init(&chip8);
    chip8_load(&chip8, buf, size);
    
    chip8.registers.I = 0x00;
    chip8.registers.V[0] = 10; // Zet de eerste register op 0x00 
    chip8.registers.V[1] = 10; // Zet de eerste register op 0x00
    chip8_exec(&chip8, 0xD015); 
    
    chip8_screen_draw_sprite(&chip8.screen, 62, 10, &chip8.memory.memory[0x00], 5); // Voorbeeld om een sprite te tekenen

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
        while (SDL_PollEvent(&event)) // zorgt ervoor dat events bv window bewegen afgehandeld kunnen worden
        {
            switch (event.type)
            {
            case SDL_QUIT:
                goto out;
                break;

            case SDL_KEYDOWN:
            {
                char key = event.key.keysym.sym;
                int virtual_key = chip8_keyboard_map(keyboard_map, key); // mapt event key naar virtual key
                if (virtual_key != -1)
                {
                    chip8_pressed_key(&chip8.keyboard, virtual_key);
                    printf("key is down %x\n", virtual_key);
                }
            }
            break;

            case SDL_KEYUP:
            {
                char key = event.key.keysym.sym;
                int virtual_key = chip8_keyboard_map(keyboard_map, key);
                if (virtual_key != -1)
                {
                    chip8_released_key(&chip8.keyboard, virtual_key);
                    printf("key is up %x\n", virtual_key);
                }
            }
            break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Maakt de achtergrond zwart dmv RGB
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0); // Kleurt de rechthoek wit

        for (int x = 0; x < CHIP8_WIDTH; x++)
        {
            for (int y = 0; y < CHIP8_HEIGHT; y++)
            {
                if (chip8_screen_is_set(&chip8.screen, x, y)) // Checkt of de pixel is gezet
                {
                    SDL_Rect rect;
                    rect.x = x * CHIP8_WINDOW_MULTIPLIER;
                    rect.y = y * CHIP8_WINDOW_MULTIPLIER;
                    rect.w = CHIP8_WINDOW_MULTIPLIER;
                    rect.h = CHIP8_WINDOW_MULTIPLIER;
                    SDL_RenderFillRect(renderer, &rect); // Teken een witte rechthoek
                }
            }
        }
        
        SDL_RenderPresent(renderer);
        if(chip8.registers.delay_timer > 0)
        {
            Sleep(100);
            chip8.registers.delay_timer-=1;
            printf("Delay timer!!!!: %d\n", chip8.registers.delay_timer);
        }
        if(chip8.registers.sound_timer > 0)
        {
            Beep(25000, 100 * chip8.registers.sound_timer); // Speel een piep geluid af
            chip8.registers.sound_timer = 0;
        }
        unsigned short opcode = chip8_memory_get_short(&chip8.memory, chip8.registers.PC);
        chip8.registers.PC += 2; // Verhoog de Program Counter met 2, omdat de instructies 2 bytes groot zijn
        chip8_exec(&chip8, opcode); // Voer de instructie uit
    }

out:
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}