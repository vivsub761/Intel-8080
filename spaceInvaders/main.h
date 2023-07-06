#include <stdio.h>
#include "SDL.h"
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    RUNNING,
    PAUSED,
    IDLE
} state;

typedef struct flags {
    u_int8_t zero;
    u_int8_t sign;
    u_int8_t parity;
    u_int8_t carry;
    u_int8_t auxCarry;
    u_int8_t interrupt_enabled;
} flags;

typedef struct {
    u_int8_t a;
    u_int8_t b;
    u_int8_t c;
    u_int8_t d;
    u_int8_t e;
    u_int8_t h;
    u_int8_t l;
    u_int16_t sp;    
    u_int16_t pc; 
    u_int8_t mem[0x10000];
    u_int8_t enable;
    u_int8_t zpad: 3;
    state state;
    struct flags flags;
} Chip;





typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
} sdl;

