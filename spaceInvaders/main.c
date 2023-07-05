#include <stdio.h>
#include "opCodeHandlers.c"
#include "SDL.h"


static void (*opCodeTable[256])();

void InitializeOpCodeTable() {
	opCodeTable[0x00] = handle0x00;
	opCodeTable[0x01] = handle0x01;
	
}

void cleanup(Chip* chip, sdl* sdl) {
	SDL_DestroyRenderer(sdl->renderer);
    SDL_DestroyWindow(sdl->window);
    SDL_Quit();
    free(chip);
    free(sdl);
}

Chip* InitializeChip() {
	Chip* chip = (Chip*) malloc(sizeof(Chip));
	chip->state = RUNNING;
	return chip;
}

bool readFileIntoChip(char* filePath, Chip* chip, int start) {
	FILE* file = fopen(filePath, "rb");
	if (file == NULL) {
		printf("File could not be found");
		return false;
	}
	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	fread(&chip->mem[start], fileSize, 1, file);
	fclose(file);
	return true;
}

void emulate(Chip* chip) {
    
    
}

void eventHandler(Chip* chip) {

}
const int width = 64;
const int height = 32;
sdl* InitializeSDL() {
	sdl* sdl = malloc(sizeof(sdl));
	SDL_Init( SDL_INIT_EVERYTHING );
	sdl->window = SDL_CreateWindow("Space Invaders!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width * 10, height * 10, SDL_WINDOW_ALLOW_HIGHDPI);
	if (sdl->window == NULL) {
        printf("COULD NOT CREATE WINDOW");
        return 0;
    }
	sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
	if (sdl->window == NULL) {
        printf("COULD NOT CREATE RENDERER");
        return 0;
    }
	return sdl;
}



int main (int argc, char**argv) {
    Chip* chip = InitializeChip();
	if (!readFileIntoChip("invaders.h", chip, 0) || !readFileIntoChip("invaders.g", chip, 0x800) 
		|| !readFileIntoChip("invaders.f", chip, 0x1000) || !readFileIntoChip("invaders.e", chip, 0x1800)) {
		printf("Unable to Read Space invaders file into chip");
		return 0;
	}
	InitializeOpCodeTable();
	sdl* sdl = InitializeSDL();

	while (chip->state != IDLE) {
		u_int8_t* op = chip->mem + chip->pc;
		opCodeTable[op[0]](chip, op);
		chip->pc += 1;
		// Run emulation
	}

	// cleanup program
	cleanup(chip, sdl);
}
	


