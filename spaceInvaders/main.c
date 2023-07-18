#include <stdio.h>
#include "opCodeHandlers.c"
#include "SDL.h"
#include "ioDevices.h"
#include <time.h>


static int (*opCodeTable[256])();

// Opcode table allows us to easily dispatch instructions by mapping opcodes to functions 
void InitializeOpCodeTable() {
	opCodeTable[0x00] = handle0x00;
	opCodeTable[0x01] = handle0x01;
	opCodeTable[0x02] = handle0x02;
	opCodeTable[0x03] = handle0x03;
	opCodeTable[0x04] = handle0x04;
	opCodeTable[0x05] = handle0x05;
	opCodeTable[0x06] = handle0x06;
	opCodeTable[0x07] = handle0x07;
	opCodeTable[0x08] = handle0x08;
	opCodeTable[0x09] = handle0x09;
	opCodeTable[0x0a] = handle0x0a;
	opCodeTable[0x0b] = handle0x0b;
	opCodeTable[0x0c] = handle0x0c;
	opCodeTable[0x0d] = handle0x0d;
	opCodeTable[0x0e] = handle0x0e;
	opCodeTable[0x0f] = handle0x0f;
	opCodeTable[0x10] = handle0x10;
	opCodeTable[0x11] = handle0x11;
	opCodeTable[0x12] = handle0x12;
	opCodeTable[0x13] = handle0x13;
	opCodeTable[0x14] = handle0x14;
	opCodeTable[0x15] = handle0x15;
	opCodeTable[0x16] = handle0x16;
	opCodeTable[0x17] = handle0x17;
	opCodeTable[0x18] = handle0x18;
	opCodeTable[0x19] = handle0x19;
	opCodeTable[0x1a] = handle0x1a;
	opCodeTable[0x1b] = handle0x1b;
	opCodeTable[0x1c] = handle0x1c;
	opCodeTable[0x1d] = handle0x1d;
	opCodeTable[0x1e] = handle0x1e;
	opCodeTable[0x1f] = handle0x1f;
	opCodeTable[0x20] = handle0x20;
	opCodeTable[0x21] = handle0x21;
	opCodeTable[0x22] = handle0x22;
	opCodeTable[0x23] = handle0x23;
	opCodeTable[0x24] = handle0x24;
	opCodeTable[0x25] = handle0x25;
	opCodeTable[0x26] = handle0x26;
	opCodeTable[0x27] = handle0x27;
	opCodeTable[0x28] = handle0x28;
	opCodeTable[0x29] = handle0x29;
	opCodeTable[0x2a] = handle0x2a;
	opCodeTable[0x2b] = handle0x2b;
	opCodeTable[0x2c] = handle0x2c;
	opCodeTable[0x2d] = handle0x2d;
	opCodeTable[0x2e] = handle0x2e;
	opCodeTable[0x2f] = handle0x2f;
	opCodeTable[0x30] = handle0x30;
	opCodeTable[0x31] = handle0x31;
	opCodeTable[0x32] = handle0x32;
	opCodeTable[0x33] = handle0x33;
	opCodeTable[0x34] = handle0x34;
	opCodeTable[0x35] = handle0x35;
	opCodeTable[0x36] = handle0x36;
	opCodeTable[0x37] = handle0x37;
	opCodeTable[0x38] = handle0x38;
	opCodeTable[0x39] = handle0x39;
	opCodeTable[0x3a] = handle0x3a;
	opCodeTable[0x3b] = handle0x3b;
	opCodeTable[0x3c] = handle0x3c;
	opCodeTable[0x3d] = handle0x3d;
	opCodeTable[0x3e] = handle0x3e;
	opCodeTable[0x3f] = handle0x3f;
	opCodeTable[0x40] = handle0x40;
	opCodeTable[0x41] = handle0x41;
	opCodeTable[0x42] = handle0x42;
	opCodeTable[0x43] = handle0x43;
	opCodeTable[0x44] = handle0x44;
	opCodeTable[0x45] = handle0x45;
	opCodeTable[0x46] = handle0x46;
	opCodeTable[0x47] = handle0x47;
	opCodeTable[0x48] = handle0x48;
	opCodeTable[0x49] = handle0x49;
	opCodeTable[0x4a] = handle0x4a;
	opCodeTable[0x4b] = handle0x4b;
	opCodeTable[0x4c] = handle0x4c;
	opCodeTable[0x4d] = handle0x4d;
	opCodeTable[0x4e] = handle0x4e;
	opCodeTable[0x4f] = handle0x4f;
	opCodeTable[0x50] = handle0x50;
	opCodeTable[0x51] = handle0x51;
	opCodeTable[0x52] = handle0x52;
	opCodeTable[0x53] = handle0x53;
	opCodeTable[0x54] = handle0x54;
	opCodeTable[0x55] = handle0x55;
	opCodeTable[0x56] = handle0x56;
	opCodeTable[0x57] = handle0x57;
	opCodeTable[0x58] = handle0x58;
	opCodeTable[0x59] = handle0x59;
	opCodeTable[0x5a] = handle0x5a;
	opCodeTable[0x5b] = handle0x5b;
	opCodeTable[0x5c] = handle0x5c;
	opCodeTable[0x5d] = handle0x5d;
	opCodeTable[0x5e] = handle0x5e;
	opCodeTable[0x5f] = handle0x5f;
	opCodeTable[0x60] = handle0x60;
	opCodeTable[0x61] = handle0x61;
	opCodeTable[0x62] = handle0x62;
	opCodeTable[0x63] = handle0x63;
	opCodeTable[0x64] = handle0x64;
	opCodeTable[0x65] = handle0x65;
	opCodeTable[0x66] = handle0x66;
	opCodeTable[0x67] = handle0x67;
	opCodeTable[0x68] = handle0x68;
	opCodeTable[0x69] = handle0x69;
	opCodeTable[0x6a] = handle0x6a;
	opCodeTable[0x6b] = handle0x6b;
	opCodeTable[0x6c] = handle0x6c;
	opCodeTable[0x6d] = handle0x6d;
	opCodeTable[0x6e] = handle0x6e;
	opCodeTable[0x6f] = handle0x6f;
	opCodeTable[0x70] = handle0x70;
	opCodeTable[0x71] = handle0x71;
	opCodeTable[0x72] = handle0x72;
	opCodeTable[0x73] = handle0x73;
	opCodeTable[0x74] = handle0x74;
	opCodeTable[0x75] = handle0x75;
	opCodeTable[0x76] = handle0x76;
	opCodeTable[0x77] = handle0x77;
	opCodeTable[0x78] = handle0x78;
	opCodeTable[0x79] = handle0x79;
	opCodeTable[0x7a] = handle0x7a;
	opCodeTable[0x7b] = handle0x7b;
	opCodeTable[0x7c] = handle0x7c;
	opCodeTable[0x7d] = handle0x7d;
	opCodeTable[0x7e] = handle0x7e;
	opCodeTable[0x7f] = handle0x7f;
	opCodeTable[0x80] = handle0x80;
	opCodeTable[0x81] = handle0x81;
	opCodeTable[0x82] = handle0x82;
	opCodeTable[0x83] = handle0x83;
	opCodeTable[0x84] = handle0x84;
	opCodeTable[0x85] = handle0x85;
	opCodeTable[0x86] = handle0x86;
	opCodeTable[0x87] = handle0x87;
	opCodeTable[0x88] = handle0x88;
	opCodeTable[0x89] = handle0x89;
	opCodeTable[0x8a] = handle0x8a;
	opCodeTable[0x8b] = handle0x8b;
	opCodeTable[0x8c] = handle0x8c;
	opCodeTable[0x8d] = handle0x8d;
	opCodeTable[0x8e] = handle0x8e;
	opCodeTable[0x8f] = handle0x8f;
	opCodeTable[0x90] = handle0x90;
	opCodeTable[0x91] = handle0x91;
	opCodeTable[0x92] = handle0x92;
	opCodeTable[0x93] = handle0x93;
	opCodeTable[0x94] = handle0x94;
	opCodeTable[0x95] = handle0x95;
	opCodeTable[0x96] = handle0x96;
	opCodeTable[0x97] = handle0x97;
	opCodeTable[0x98] = handle0x98;
	opCodeTable[0x99] = handle0x99;
	opCodeTable[0x9a] = handle0x9a;
	opCodeTable[0x9b] = handle0x9b;
	opCodeTable[0x9c] = handle0x9c;
	opCodeTable[0x9d] = handle0x9d;
	opCodeTable[0x9e] = handle0x9e;
	opCodeTable[0x9f] = handle0x9f;
	opCodeTable[0xa0] = handle0xa0;
	opCodeTable[0xa1] = handle0xa1;
	opCodeTable[0xa2] = handle0xa2;
	opCodeTable[0xa3] = handle0xa3;
	opCodeTable[0xa4] = handle0xa4;
	opCodeTable[0xa5] = handle0xa5;
	opCodeTable[0xa6] = handle0xa6;
	opCodeTable[0xa7] = handle0xa7;
	opCodeTable[0xa8] = handle0xa8;
	opCodeTable[0xa9] = handle0xa9;
	opCodeTable[0xaa] = handle0xaa;
	opCodeTable[0xab] = handle0xab;
	opCodeTable[0xac] = handle0xac;
	opCodeTable[0xad] = handle0xad;
	opCodeTable[0xae] = handle0xae;
	opCodeTable[0xaf] = handle0xaf;
	opCodeTable[0xb0] = handle0xb0;
	opCodeTable[0xb1] = handle0xb1;
	opCodeTable[0xb2] = handle0xb2;
	opCodeTable[0xb3] = handle0xb3;
	opCodeTable[0xb4] = handle0xb4;
	opCodeTable[0xb5] = handle0xb5;
	opCodeTable[0xb6] = handle0xb6;
	opCodeTable[0xb7] = handle0xb7;
	opCodeTable[0xb8] = handle0xb8;
	opCodeTable[0xb9] = handle0xb9;
	opCodeTable[0xba] = handle0xba;
	opCodeTable[0xbb] = handle0xbb;
	opCodeTable[0xbc] = handle0xbc;
	opCodeTable[0xbd] = handle0xbd;
	opCodeTable[0xbe] = handle0xbe;
	opCodeTable[0xbf] = handle0xbf;
	opCodeTable[0xc0] = handle0xc0;
	opCodeTable[0xc1] = handle0xc1;
	opCodeTable[0xc2] = handle0xc2;
	opCodeTable[0xc3] = handle0xc3;
	opCodeTable[0xc4] = handle0xc4;
	opCodeTable[0xc5] = handle0xc5;
	opCodeTable[0xc6] = handle0xc6;
	opCodeTable[0xc7] = handle0xc7;
	opCodeTable[0xc8] = handle0xc8;
	opCodeTable[0xc9] = handle0xc9;
	opCodeTable[0xca] = handle0xca;
	opCodeTable[0xcb] = handle0xcb;
	opCodeTable[0xcc] = handle0xcc;
	opCodeTable[0xcd] = handle0xcd;
	opCodeTable[0xce] = handle0xce;
	opCodeTable[0xcf] = handle0xcf;
	opCodeTable[0xd0] = handle0xd0;
	opCodeTable[0xd1] = handle0xd1;
	opCodeTable[0xd2] = handle0xd2;
	opCodeTable[0xd3] = handle0xd3;
	opCodeTable[0xd4] = handle0xd4;
	opCodeTable[0xd5] = handle0xd5;
	opCodeTable[0xd6] = handle0xd6;
	opCodeTable[0xd7] = handle0xd7;
	opCodeTable[0xd8] = handle0xd8;
	opCodeTable[0xd9] = handle0xd9;
	opCodeTable[0xda] = handle0xda;
	opCodeTable[0xdb] = handle0xdb;
	opCodeTable[0xdc] = handle0xdc;
	opCodeTable[0xdd] = handle0xdd;
	opCodeTable[0xde] = handle0xde;
	opCodeTable[0xdf] = handle0xdf;
	opCodeTable[0xe0] = handle0xe0;
	opCodeTable[0xe1] = handle0xe1;
	opCodeTable[0xe2] = handle0xe2;
	opCodeTable[0xe3] = handle0xe3;
	opCodeTable[0xe4] = handle0xe4;
	opCodeTable[0xe5] = handle0xe5;
	opCodeTable[0xe6] = handle0xe6;
	opCodeTable[0xe7] = handle0xe7;
	opCodeTable[0xe8] = handle0xe8;
	opCodeTable[0xe9] = handle0xe9;
	opCodeTable[0xea] = handle0xea;
	opCodeTable[0xeb] = handle0xeb;
	opCodeTable[0xec] = handle0xec;
	opCodeTable[0xed] = handle0xed;
	opCodeTable[0xee] = handle0xee;
	opCodeTable[0xef] = handle0xef;
	opCodeTable[0xf0] = handle0xf0;
	opCodeTable[0xf1] = handle0xf1;
	opCodeTable[0xf2] = handle0xf2;
	opCodeTable[0xf3] = handle0xf3;
	opCodeTable[0xf4] = handle0xf4;
	opCodeTable[0xf5] = handle0xf5;
	opCodeTable[0xf6] = handle0xf6;
	opCodeTable[0xf7] = handle0xf7;
	opCodeTable[0xf8] = handle0xf8;
	opCodeTable[0xf9] = handle0xf9;
	opCodeTable[0xfa] = handle0xfa;
	opCodeTable[0xfb] = handle0xfb;
	opCodeTable[0xfc] = handle0xfc;
	opCodeTable[0xfd] = handle0xfd;
	opCodeTable[0xfe] = handle0xfe;
	opCodeTable[0xff] = handle0xff;
}

// After emulation is finished, cleanup by freeing allocated memory and destroying appropriate objects
void cleanup(Chip* chip, sdl* sdl) {
	SDL_DestroyRenderer(sdl->renderer);
    SDL_DestroyWindow(sdl->window);
    SDL_Quit();
    free(chip);
    free(sdl);
}
// Initialize chip object
Chip* InitializeChip() {
	Chip* chip = (Chip*) malloc(sizeof(Chip));
	chip->state = RUNNING;
	return chip;
}

// Read file into chip memory starting at a certain index in memory
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
// perform interrupt if enabled
float interrupt(Chip* chip, u_int8_t* op, u_int8_t opCode) {
	if (chip->interrupt_enabled == 1) {
		chip->interrupt_enabled = 0;
		opCodeTable[opCode](chip, op);
		return time(NULL);
	}
	return -1;

}

// emulated for 33332 cycles, generate interrupts periodically
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
		if (op[0] != 0xc7) {
			chip->pc += 1;
		}
	}
}

// handles keyboard presses
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


// Initializes sdl renderer and window
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

// iterates through 0x2400-0x3fff(where the VRAM is stored) and renders it on the sdl screen
void renderGraphics(Chip* chip, sdl* sdl) {
	SDL_Color pixelColor;
    SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 0);
    SDL_RenderClear(sdl->renderer);
	uint16_t offset = 0x2400;
    for(uint16_t r=0; r<224; r++){
    	for(int16_t c=0; c<256; c++){
        	uint16_t Yoffset = 0x20 * r;
        	uint16_t Xoffset = (c >> 3);
        	uint16_t current_byte = offset + Xoffset + Yoffset;
        	uint8_t current_bit = (c % 8);

        	bool thisPixel = (chip->mem[current_byte]& (1 << current_bit)) >> current_bit;
			SDL_SetRenderDrawColor(sdl->renderer, thisPixel ? 255 : 0, thisPixel ? 255 : 0, thisPixel ? 255 : 0, thisPixel ? 255 : 0);
        	SDL_RenderDrawPoint(sdl->renderer, r, 255 - c);
    	}
    }
    SDL_RenderPresent(sdl->renderer);
}

// Main function, create chip, read files into it, create sdl, run emulation loop
int main (int argc, char**argv) {
    Chip* chip = InitializeChip();
	chip->devices = InitializeDevices();
	if (!readFileIntoChip("invaders.h", chip, 0) || !readFileIntoChip("invaders.g", chip, 0x800) 
		|| !readFileIntoChip("invaders.f", chip, 0x1000) || !readFileIntoChip("invaders.e", chip, 0x1800)) {
		printf("Unable to Read Space invaders file into chip");
		return 0;
	}
	// readFileIntoChip("cpudiag.bin", chip, 0x100);
	// chip->pc = 0x100;
	InitializeOpCodeTable();
	sdl* sdl = InitializeSDL();
	int summed = 0;
	// chip->mem[368] = 0x7;
	// chip->mem[0x59c] = 0xc3;
	// chip->mem[0x59d] = 0xc2;
	// chip->mem[0x59e] = 0x05;
	
	SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 1);
    SDL_RenderClear(sdl->renderer);
	while (chip->state != IDLE) {
		eventHandler(chip);
		if (chip->state == PAUSED) {
			continue;
		}
		u_int8_t* op = chip->mem + chip->pc;
		emulate(chip);
		renderGraphics(chip, sdl);
	}
	// cleanup program
	cleanup(chip, sdl);

}
	


