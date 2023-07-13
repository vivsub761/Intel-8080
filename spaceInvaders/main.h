#include "SDL.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ioDevices.h"

#ifndef MAIN_H
#define MAIN_H
typedef enum {
    RUNNING,
    PAUSED,
    IDLE
} state;

typedef struct flags {
    u_int8_t zero: 1;
    u_int8_t sign: 1;
    u_int8_t parity: 1;
    u_int8_t carry: 1;
    u_int8_t auxCarry: 1;
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
    u_int8_t interrupt_enabled;
    u_int8_t mem[0x4000];
    u_int8_t zpad: 3;
    state state;
    struct flags flags;
    ioDevices* devices;
} Chip;




typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
} sdl;
#endif