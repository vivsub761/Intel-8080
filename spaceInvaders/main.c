#include <stdio.h>
#include "opCodeHandlers.c"
#include "SDL.h"
#include "ioDevices.h"
#include <time.h>


static int (*opCodeTable[256])();

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
	opCodeTable[0xd7] = handle0xd7;
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
	int bytesRead = fread(&chip->mem[start], 1, fileSize, file);
	if (bytesRead != fileSize) {
		printf("Could not read the file fully\n");
		return false;
	}
	fclose(file);
	return true;
}
float interrupt(Chip* chip, u_int8_t* op, u_int8_t opCode) {
	if (chip->flags.interrupt_enabled == 1) {
		chip->flags.interrupt_enabled = 0;
		opCodeTable[opCode](chip, op);
		chip->pc -= 1;
		return time(NULL);
	}
	return -1;

}

void emulate(Chip* chip) {
    int cycles = 0;
	float lastInterrupt = 0;
	while (cycles < 33332) {
		u_int8_t* op = chip->mem + chip->pc;
		// printf("PC: 0x%04X, ", chip->pc);
		// printf("OPCODE: 0x%02X: ", op[0]);
		cycles += opCodeTable[op[0]](chip, op);
		
		if (time(NULL) - lastInterrupt > 1.0/60.0) {
			float timing = interrupt(chip, op, 0xd7);
			lastInterrupt = timing == -1 ? lastInterrupt : timing;
		}
		printf("\n");
		chip->pc += 1;
	}
}

void eventHandler(Chip* chip) {
	SDL_Event windowEvent;
	while (SDL_PollEvent(&windowEvent)) {
		switch(windowEvent.type) {
			case SDL_QUIT:
                chip->state = IDLE;
                return;
			case SDL_KEYDOWN:
				switch(windowEvent.key.keysym.sym) {
					case SDLK_ESCAPE:
						// stop emulation
                        chip->state = IDLE;
                        return;
					case SDLK_p:
						// PAUSE EMULATION
						chip->state = chip->state == RUNNING ? PAUSED : RUNNING;
					case SDLK_LEFT:
						// move left, set 5th bit to 1
						chip->devices->read1 |= 0b00100000;
					case SDLK_RIGHT:
						// move right, set 6th bit to 1
						chip->devices->read1 |= 0b01000000;
					case SDLK_SPACE:
						// shoot, set 4th bit to 1
						chip->devices->read1 |= 0b00010000;
					case SDLK_c:
						// coin inserted, set 0th bit to 1
						chip->devices->read1 |= 0b00000001;
					case SDLK_RETURN:
						// Player 1 Start, set 2nd bit to 1
						chip->devices->read1 |= 0b00000100;
					default:
						break;
				}
			case SDL_KEYUP:
				switch(windowEvent.key.keysym.sym) {
					case SDLK_LEFT:
						chip->devices->read1 &= 0b11011111; 

					case SDLK_RIGHT:
						chip->devices->read1 &= 0b10111111;
					case SDLK_SPACE:
						chip->devices->read1 &= 0b11101111;
					case SDLK_c:
						chip->devices->read1 &= 0x11111110;
					case SDLK_RETURN:
						chip->devices->read1 &= 0x11111011;
					default:
						break;
				}
				
				
			
		}
	}
}


const int width = 64;
const int height = 32;
sdl* InitializeSDL() {
	sdl* sdl = malloc(sizeof(sdl));
	SDL_Init( SDL_INIT_EVERYTHING );
	sdl->window = SDL_CreateWindow("Space Invaders!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 224, 256, 0);
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


void renderGraphics(Chip* chip, sdl* sdl) {
	
}


int main (int argc, char**argv) {
    Chip* chip = InitializeChip();
	chip->devices = InitializeDevices();
	if (!readFileIntoChip("invaders.h", chip, 0) || !readFileIntoChip("invaders.g", chip, 0x800) 
		|| !readFileIntoChip("invaders.f", chip, 0x1000) || !readFileIntoChip("invaders.e", chip, 0x1800)) {
		printf("Unable to Read Space invaders file into chip");
		return 0;
	}
	InitializeOpCodeTable();
	sdl* sdl = InitializeSDL();
	int summed = 0;
	
	// SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 1);
    // SDL_RenderClear(sdl->renderer);
	int i = 1;
	// opCodeTable[0x02](chip, chip->mem + chip->pc);
	// while (chip->state != IDLE) {
	// 	eventHandler(chip);
	// 	if (chip->state == PAUSED) {
	// 		continue;
			
	// 	}
	// 	u_int8_t* op = chip->mem + chip->pc;
	// 	printf("INSTRUCTION NUMBER: %d \n", i++);
	// 	printf("PC: 0x%04X, ", chip->pc);
	// 	printf("OPCODE: 0x%02X: ", op[0]);
	// 	opCodeTable[op[0]](chip, op);
	// 	printf("\n");
	// 	chip->pc += 1;
	// 	emulate(chip);
	// 	renderGraphics(chip, sdl);
	// }
	// printf("\n\nSUMMED BEFORE: %d\n\n", summed);
	// int summed2 = 0;
	// for (int i = 0x2400; i < 0x3fff + 1; i++) {
	// 	summed2 += chip->mem[i];
	// }
	// printf("SUMMED AFTER: %d", summed2);
	// for (int i = 1; i < 50; i++) {
	// 	u_int8_t* op = chip->mem + chip->pc;
	// 	printf("INSTRUCTION NUMBER: %d \n", i);
	// 	printf("PC: 0x%04X, ", chip->pc);
	// 	printf("OPCODE: 0x%02X: ", op[0]);
	// 	opCodeTable[op[0]](chip, op);
	// 	printf("\n");
		// printf("A: 0x%04X\n", chip->a);
		// printf("B: 0x%04X\n", chip->b);
		// printf("C: 0x%04X\n", chip->c);
		// printf("D: 0x%04X\n", chip->d);
		// printf("E: 0x%04X\n", chip->e);
		// printf("H: 0x%04X\n", chip->h);
		// printf("L: 0x%04X\n", chip->l);
		// printf("CARRY FLAG: 0x%04X\n", chip->flags.carry);
		// printf("SIGN FLAG: 0x%04X\n", chip->flags.sign);
		// printf("PARITY FLAG: 0x%04X\n", chip->flags.parity);
		// printf("ZERO FLAG: 0x%04X\n", chip->flags.zero);


	// 	chip->pc += 1;
	// }

	// cleanup program
	cleanup(chip, sdl);
}
	


