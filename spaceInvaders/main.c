#include <stdio.h>
#include "opCodeHandlers.c"
#include "SDL.h"


static void (*opCodeTable[256])();

void InitializeOpCodeTable() {
	opCodeTable[0x00] = handle0x00;
	opCodeTable[0x01] = handle0x01;
	// opCodeTable[0x02] = handle0x02;
	// opCodeTable[0x03] = handle0x03;
	// opCodeTable[0x04] = handle0x02;
	opCodeTable[0x05] = handle0x05;
	opCodeTable[0x06] = handle0x06;
	// opCodeTable[0x07] = handle0x02;
	// opCodeTable[0x08] = handle0x02;
	opCodeTable[0x09] = handle0x09;
	// opCodeTable[0x0a] = handle0x02;
	// opCodeTable[0x0b] = handle0x02;
	// opCodeTable[0x0c] = handle0x02;
	opCodeTable[0x0d] = handle0x0d;
	opCodeTable[0x0e] = handle0x0e;
	opCodeTable[0x0f] = handle0x0f;
	opCodeTable[0x11] = handle0x11;
	opCodeTable[0x13] = handle0x13;
	opCodeTable[0x19] = handle0x19;
	opCodeTable[0x1a] = handle0x1a;
	opCodeTable[0x21] = handle0x21;
	opCodeTable[0x23] = handle0x23;
	opCodeTable[0x26] = handle0x26;
	opCodeTable[0x29] = handle0x29;
	opCodeTable[0x31] = handle0x31;
	opCodeTable[0x32] = handle0x32;
	opCodeTable[0x36] = handle0x36;
	opCodeTable[0x3a] = handle0x3a;
	opCodeTable[0x3e] = handle0x3e;
	opCodeTable[0x56] = handle0x56;
	opCodeTable[0x5e] = handle0x5e;
	opCodeTable[0x66] = handle0x66;
	opCodeTable[0x6f] = handle0x6f;
	opCodeTable[0x77] = handle0x77;
	opCodeTable[0x7a] = handle0x7a;
	opCodeTable[0x7b] = handle0x7b;
	opCodeTable[0x7c] = handle0x7c;
	opCodeTable[0x7e] = handle0x7e;
	opCodeTable[0xa7] = handle0xa7;
	opCodeTable[0xaf] = handle0xaf;
	opCodeTable[0xc1] = handle0xc1;
	opCodeTable[0xc2] = handle0xc2;
	opCodeTable[0xc3] = handle0xc3;
	opCodeTable[0xc5] = handle0xc5;
	opCodeTable[0xc6] = handle0xc6;
	opCodeTable[0xc9] = handle0xc9;
	opCodeTable[0xcd] = handle0xcd;
	opCodeTable[0xd1] = handle0xd1;
	opCodeTable[0xd3] = handle0xd3;
	opCodeTable[0xd5] = handle0xd5;
	opCodeTable[0xe1] = handle0xe1;
	opCodeTable[0xe5] = handle0xe5;
	opCodeTable[0xe6] = handle0xe6;
	opCodeTable[0xeb] = handle0xeb;
	opCodeTable[0xf1] = handle0xf1;
	opCodeTable[0xf5] = handle0xf5;
	opCodeTable[0xfb] = handle0xfb;
	opCodeTable[0xfe] = handle0xfe;
	
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
	rewind(file);
	// printf("FILESIZE: %d\n", fileSize);
	int bytesRead = fread(&chip->mem[start], 1, fileSize, file);
	if (bytesRead != fileSize) {
		printf("Could not read the file fully\n");
		return false;
	}
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

	// u_int8_t* op = chip->mem + chip->pc;
	// printf("PC: 0x%04X, ", chip->pc);
	// printf("OPCODE: 0x%04X: ", op[0]);
	// opCodeTable[op[0]](chip, op);
	// printf("\n");
	// for (int i = 0; i < 0x800; i++) {
	// 	printf("0x%04X\n", chip->mem[i]);
	// }
	// printf("OPCODE AT 0x18D7: 0x%02X\n", chip->mem[0x18D7]);
	for (int i = 0; i < 20; i++) {
		u_int8_t* op = &chip->mem[chip->pc];
		printf("PC: 0x%04X, ", chip->pc);
		printf("OPCODE: 0x%02X: ", op[0]);
		opCodeTable[op[0]](chip, op);
		printf("\n");
		printf("A: 0x%04X\n", chip->a);
		printf("B: 0x%04X\n", chip->b);
		printf("C: 0x%04X\n", chip->c);
		printf("D: 0x%04X\n", chip->d);
		printf("E: 0x%04X\n", chip->e);
		printf("H: 0x%04X\n", chip->h);
		printf("L: 0x%04X\n", chip->l);

		
		chip->pc += 1;
	}
	// printf("OPCODE AT 0x18D7: 0x%02X", chip->mem[0x18D7]);
	// while (chip->state != IDLE) {
	// 	u_int8_t* op = chip->mem + chip->pc;
	// 	opCodeTable[op[0]](chip, op);
	// 	chip->pc += 1;
	// 	// Run emulation
	// }

	// cleanup program
	cleanup(chip, sdl);
}
	


