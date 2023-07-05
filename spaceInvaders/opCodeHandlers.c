#include "main.h"
#include "helperFunctions.c"



// NOP
void handle0x00(Chip* chip, u_int8_t* op) {
    printf("NO OP");
    return;
}
// LXI B,d16
void handle0x01(Chip* chip, u_int8_t* op) {
    printf("LXI B d16: SET REGISTER B to 0x%04X and REGISTER C to 0x%04X", op[2], op[1]);
    chip->b = op[2];
    chip->c = op[1];
    chip->pc += 2;
}
// STAX B
// Set address obtained by combining B and C registers to A in memory
void handle0x02(Chip* chip, u_int8_t* op) {
    u_int16_t address = getBC(chip);
    printf("STAX B: Set memory at address 0x%04X to value in register A: 0x%04X", address, chip->a);
    chip->mem[address] = chip->a;
}
// INX B
// increment BC register pair
void handle0x03(Chip* chip, u_int8_t* op) {
    u_int16_t bc = getBC(chip) + 1;
    printf("INX B: INCREMENT BC RESIGER PAIR TO 0x%04X", bc);
    setBandCtoBC(chip, bc);

}
// INR B
// increment B register
void handle0x04(Chip* chip, u_int8_t* op) {
    prinf("INR B: INCREMENT B REGISTER TO 0x%04X", chip->b + 1);
    u_int16_t carryCheck = chip->b + 1;
    chip->b += 1;
    setFlags8Bits(chip, chip->b);
}
// DCR B
// decrement B register
void handle0x05(Chip* chip, u_int8_t* op) {
    prinf("DCR B: DECREMENT B REGISTER TO 0x%04X", chip->b - 1);
    chip->b -= 1;
    setFlags8Bits(chip, chip->b);
}
// MVI B,d8
// set register b to immediate
void handle0x06(Chip* chip, u_int8_t* op) {
    printf("MVI B,d8: SET REGSITER B TO 0x%04X", op[1]);
    chip->b = op[1];
    chip->pc += 1;
}
// RLC
// shift a register left by one bit. the leftmost bit is put into the carry flag. The previous value of the carry flag is put in the rightmost bit of the a register
void handle0x07(Chip* chip, u_int8_t* op) {
    
    u_int8_t leftMostBit = chip->a >> 7;
    chip->flags->carry = leftMostBit;
    chip->a = (chip->a << 1) | leftMostBit;
    printf("RLC: SHIFT REGISTER A LEFT ONE BIT. PREVIOUS LEFTMOST BIT PUT IN CARRY FLAG, Leftmost BIt put in rightmost bit, CARRY FLAG: 0x%04X, A RESGISTER: 0x%04X", chip->flags->carry, chip->a);

}

void handle0x08(Chip* chip, u_int8_t* op) {
    printf("NO OP");
}
// DAD B
// HL = HL + BC
void handle0x09(Chip* chip, u_int8_t* op) {
    u_int16_t hl = getHL(chip);
    u_int16_t bc = getBC(chip);
    u_int32_t carryCheck = hl + bc;
    u_int16_t res = hl + bc;
    setHandLtoHL(chip, res);
    printf("DAD B: The value of the HL register pair: 0x%04X is added to the value of the BC Regsiter PAIR: 0x%04X and the result 0x%04X is put into the HL register pair", hl, bc, res);
    chip->flags->carry = carryCheck > (pow(2, 16) - 1);
}
// LDAX B
// Load memory at address stored in BC register into A register
void handle0x0a(Chip* chip, u_int8_t* op) {
    u_int16_t address = getBC(chip);
    chip->a = chip->mem[address];
    printf("LDAX B: Register A set to value 0x%04X at address 0x%04X in memory", chip->a, address);
}

// decrement BC register pair
// DCX B
void handle0x0b(Chip* chip, u_int8_t* op) {
    u_int16_t bc = getBC(chip);
    bc -= 1;
    setBandCtoBC(chip, bc);
    printf("DCX B: Decrement BC register pair to 0x%04X", getBC(chip));
}
// INR C
// Register C += 1;
void handle0x0c(Chip* chip, u_int8_t* op) {
    chip->c += 1;
    setFlags8Bits(chip, chip->c);
    printf("INR C: Increment Register C to 0x%04X", chip->c);
}
// DCR C
// Register C -= 1
void handle0x0d(Chip* chip, u_int8_t* op) {
    chip->c -= 1;
    setFlags8Bits(chip, chip->c);
    printf("DCR C: Decrement Register C to 0x%04X", chip->c);
}

// MVI C, d8
// Move immediate to C register
void handle0x0e(Chip* chip, u_int8_t* op) {
    chip->c = op[1];
    chip->pc += 1;
    printf("MVI C, d8: Move immediate 0x%04X to Register C", op[1]);
}

// RRC
// Rightmost bit of Register A put into carry flag
// register A shifted one bit to right, prevous carry flag put in leftmost bit
void handle0x0f(Chip* chip, u_int8_t* op) {
    chip->flags->carry = chip->a & 1;
    chip->a = (chip->a >> 1) | (chip->flags->carry << 7);
    printf("RRC: SHIFT REGISTER A RIGHT ONE BIT. PREVIOUS RIGHTMOST BIT PUT IN CARRY FLAG, prev Rightmost bit PUT IN LEFTMOST BIT OF A REGISTER, CARRY FLAG: 0x%04X, A RESGISTER: 0x%04X", chip->flags->carry, chip->a);
}
// NOP
void handle0x10(Chip* chip, u_int8_t* op) {
    printf("NOP");
}
// LXI D,d16
// Load 16 bit immediate into DE register pair
void handle0x11(Chip* chip, u_int8_t* op) {
    chip->d = op[2];
    chip->e = op[1];
    chip->pc += 2;
    printf("LXI D,d16: Load 16 bit immediate into DE register pair. Register D = 0x%04X, Register E = 0x%04X", op[2], op[1]);
}
// STAX D
// Store contents of register A in memory address at DE register pair
void handle0x12(Chip* chip, u_int8_t* op) {
    u_int16_t address = getDE(chip);
    chip->mem[address] = chip->a;
    printf("STAX D: Set memory at address 0x%04X to value in register A: 0x%04X", address, chip->a);
}

// INX D
// Increment DE register Pair
void handle0x13(Chip* chip, u_int8_t* op) {
    u_int16_t de = getDE(chip) + 1;
    printf("INX D: INCREMENT DE RESIGER PAIR TO 0x%04X", de);
    setBandCtoBC(chip, de);
}

// INR D
// increment D register
void handle0x14(Chip* chip, u_int8_t* op) {
    prinf("INR D: INCREMENT D REGISTER TO 0x%04X", chip->d + 1);
    chip->d += 1;
    setFlags8Bits(chip, chip->d);
}

// DCR D
// decrement D register
void handle0x15(Chip* chip, u_int8_t* op) {
    prinf("DCR D: DECREMENT D REGISTER TO 0x%04X", chip->d - 1);
    chip->d -= 1;
    setFlags8Bits(chip, chip->d);
}

// MVI D,d8
// set register d to immediate
void handle0x16(Chip* chip, u_int8_t* op) {
    printf("MVI D,d8: SET REGSITER D TO 0x%04X", op[1]);
    chip->d = op[1];
    chip->pc += 1;
}
// RAL
void handle0x17(Chip* chip, u_int8_t* op) {
    u_int8_t carryFlag = chip->flags->carry;
    chip->flags->carry = chip->a >> 7;
    chip->a = (chip->a << 1) | carryFlag;
    printf("RLC: SHIFT REGISTER A LEFT ONE BIT. PREVIOUS LEFTMOST BIT PUT IN CARRY FLAG, Previous carry bit put in rightmost bit, CARRY FLAG: 0x%04X, A RESGISTER: 0x%04X", chip->flags->carry, chip->a);
}

void handle0x18(Chip* chip, u_int8_t* op) {
    printf("NOP");
}

// DAD D
// HL = HL + DE
void handle0x19(Chip* chip, u_int8_t* op) {
    u_int16_t hl = getHL(chip);
    u_int16_t de = getDE(chip);
    u_int32_t carryCheck = hl + de;
    u_int16_t res = hl + de;
    setHandLtoHL(chip, res);
    printf("DAD D: The value of the HL register pair: 0x%04X is added to the value of the DE Regsiter PAIR: 0x%04X and the result 0x%04X is put into the HL register pair", hl, de, res);
    chip->flags->carry = carryCheck > (pow(2, 16) - 1);
}

// LDAX D
// Load memory at address stored in DE register into A register
void handle0x1a(Chip* chip, u_int8_t* op) {
    u_int16_t address = getDE(chip);
    chip->a = chip->mem[address];
    printf("LDAX D: Register A set to value 0x%04X at address 0x%04X in memory", chip->a, address);
}

// decrement DE register pair
// DCX D
void handle0x1b(Chip* chip, u_int8_t* op) {
    u_int16_t de = getDE(chip);
    de -= 1;
    setBandCtoBC(chip, de);
    printf("DCX D: Decrement BC register pair to 0x%04X", getDE(chip));
}

// INR E
// Register E += 1;
void handle0x1c(Chip* chip, u_int8_t* op) {
    chip->e += 1;
    setFlags8Bits(chip, chip->e);
    printf("INR E: Increment Register E to 0x%04X", chip->e);
}

// DCR E
// Register E -= 1
void handle0x1d(Chip* chip, u_int8_t* op) {
    chip->e -= 1;
    setFlags8Bits(chip, chip->e);
     printf("DCR E: Decrement Register E to 0x%04X", chip->e);
}

// MVI E, d8
// Move immediate to E register
void handle0x1e(Chip* chip, u_int8_t* op) {
    chip->e = op[1];
    chip->pc += 1;
    printf("MVI E, d8: Move immediate 0x%04X to Register E", op[1]);
}
// RAR
void handle0x1f(Chip* chip, u_int8_t* op) {
    u_int8_t carryFlag = chip->flags->carry;
    chip->flags->carry = chip->a & 1;
    chip->a = (chip->a >> 1) | (carryFlag << 7);
    printf("SHIFT REGISTER A ONE BIT TO THE RIGHT, PREV RIGHTMOST BIT PUT INTO CARRY FLAG, PREV CARRY FLAG PUT INTO LEFTMOST BIT. Register A: 0x%04X, Carry FLAG: 0x%04X ", chip->a, chip->flags->carry);
}
// NOP
void handle0x20(Chip* chip, u_int8_t* op) {
    printf("NOP");
}

// LXI H,d16
// Load 16 bit immediate into HL register pair
void handle0x21(Chip* chip, u_int8_t* op) {
    chip->h = op[2];
    chip->l = op[1];
    chip->pc += 2;
    printf("LXI H,d16: Load 16 bit immediate into HL register pair. Register H = 0x%04X, Register L = 0x%04X", op[2], op[1]);
}

// SHLD a16
// save register L at address a16 and register H at address a16 + 1
void handle0x22(Chip* chip, u_int8_t* op) {
    u_int16_t address = (op[2] << 8) | op[1];
    chip->mem[address] = chip->l;
    chip->mem[address + 1] = chip->h;
    printf("SAVING REGISTER L: 0x%04X at address 0x%04X and REGISTER H: 0x%04X at address 0x%04X", chip->l, address, chip->h, address + 1);
}

// INX H
// Increment HL register Pair
void handle0x23(Chip* chip, u_int8_t* op) {
    u_int16_t hl = getHL(chip) + 1;
    printf("INX H: INCREMENT HL RESIGER PAIR TO 0x%04X", hl);
    setHandLtoHL(chip, hl);
}

// INR H
// increment H register
void handle0x24(Chip* chip, u_int8_t* op) {
    prinf("INR H: INCREMENT H REGISTER TO 0x%04X", chip->h + 1);
    chip->h += 1;
    setFlags8Bits(chip, chip->h);
}

// DCR H
// decrement H register
void handle0x25(Chip* chip, u_int8_t* op) {
    prinf("DCR H: DECREMENT H REGISTER TO 0x%04X", chip->h - 1);
    chip->h -= 1;
    setFlags8Bits(chip, chip->h);
}

// MVI H,d8
// set register H to immediate
void handle0x26(Chip* chip, u_int8_t* op) {
    printf("MVI H,d8: SET REGSITER H TO 0x%04X", op[1]);
    chip->h = op[1];
    chip->pc += 1;
}

// DAA
void handle0x27(Chip* chip, u_int8_t* op) {
    printf("DAD: STILL UNIMPLEMENTED");
}

void handle0x28(Chip* chip, u_int8_t* op) {
    printf("NOP");
}

// DAD H
// HL = HL + DE
void handle0x29(Chip* chip, u_int8_t* op) {
    u_int16_t hl = getHL(chip);
    u_int32_t carryCheck = hl + hl;
    u_int16_t res = hl + hl;
    setHandLtoHL(chip, res);
    printf("DAD H: The value of the HL register pair: 0x%04X is added to itself and the result 0x%04X is put into the HL register pair", hl, res);
    chip->flags->carry = carryCheck > (pow(2, 16) - 1);
}

// LHLD a16
void handle0x2a(Chip* chip, u_int8_t* op) {
    u_int16_t address = (op[2] << 8) | op[1];
    chip->l = chip->mem[address];
    chip->h = chip->mem[address + 1];
    printf("REGISTER L: 0x%04X SET TO value at address 0x%04X, REGISTER H: 0x%04X SET TO value at address 0x%04X", chip->l, address, chip->h, address + 1);
}

// decrement HL register pair
// DCX H
void handle0x2b(Chip* chip, u_int8_t* op) {
    u_int16_t hl = getHL(chip);
    hl -= 1;
    setBandCtoBC(chip, hl);
    printf("DCX H: Decrement HL register pair to 0x%04X", getHL(chip));
}

// INR L
// Register L += 1;
void handle0x2c(Chip* chip, u_int8_t* op) {
    chip->l += 1;
    setFlags8Bits(chip, chip->l);
    printf("INR L: Increment Register L to 0x%04X", chip->l);
}

// DCR L
// decrement L register
void handle0x2d(Chip* chip, u_int8_t* op) {
    prinf("DCR L: DECREMENT L REGISTER TO 0x%04X", chip->l - 1);
    chip->l -= 1;
    setFlags8Bits(chip, chip->l);
}

// MVI L, d8
// Move immediate to L register
void handle0x2e(Chip* chip, u_int8_t* op) {
    chip->l = op[1];
    chip->pc += 1;
    printf("MVI L, d8: Move immediate 0x%04X to Register L", op[1]);
}
// CMA
void handle0x2f(Chip* chip, u_int8_t* op) {
    chip->a = ~(chip->a);
    printf("CMA: INVERT the bits in Register A: 0x%04X", chip->a);
}
// NOP
void handle0x30(Chip* chip, u_int8_t* op) {
    printf("NOP");
}
// LXI SP,d16
void handle0x31(Chip* chip, u_int8_t* op) {
    chip->sp = (op[2] << 8) | op[1];
    printf("LXI SP,d16: change stack pointer to immediate 0x%04X", chip->sp);
    chip->pc += 2;
}
// STA a16
void handle0x32(Chip* chip, u_int8_t* op) {
    u_int16_t address = (op[2] << 8) | op[1];
    printf("STA a16: Store the value in register A: 0x%04X at address 0x%04X", chip->a, address);
    chip->mem[address] = chip->a;
    chip->pc += 2;
}

// MVI M,d8
// Move the immediate to the address the HL pair points to 
void handle0x36(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    printf("MVI B,d8: SET Memory at address 0x%04X TO 0x%04X", address, op[1]);
    chip->mem[address] = op[1];
    chip->pc += 1;
}
// LDA a16
void handle0x3a(Chip* chip, u_int8_t* op) {
    u_int16_t address = (op[2] << 8) | op[1];
    chip->a = chip->mem[address];
    printf("LOAD VALUE(0x%04X) from address 0x%04X into Register A", address, chip->a); 
    chip->pc += 2;
}

// MVI A, d8
// Move immediate to A register
void handle0x3e(Chip* chip, u_int8_t* op) {
    chip->a = op[1];
    chip->pc += 1;
    printf("MVI A, d8: Move immediate 0x%04X to Register A", op[1]);
}
// MOV D,M
// Set Register D to value pointed at by HL register
void handle0x56(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->d = chip->mem[address];
    printf("MOV D,M: SET REGISTER D(0x%04X) to value at address 0x%04X", chip->d, address);
}
// MOV E,M
// Set Register E to value pointed at by HL register
void handle0x5e(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->e = chip->mem[address];
    printf("MOV E,M: SET REGISTER E(0x%04X) to value at address 0x%04X", chip->e, address);
}

// MOV H,M
// Set Register E to value pointed at by HL register
void handle0x66(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->h = chip->mem[address];
    printf("MOV H,M: SET REGISTER H(0x%04X) to value at address 0x%04X", chip->h, address);
}
// MOV L, A
void handle0x6f(Chip* chip, u_int8_t* op) {
    chip->a = chip->l;
    printf("MOV L, A: MOVE VALUE FROM REGISTER L(0x%04X) to register A",chip->a);
}
// 	MOV M,A
void handle0x77(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->mem[address] = chip->a;
    printf("MOV M, A: SET VALUE AT ADDRESS 0x%04X to register A: (0x%04X)", address, chip->a);
}

// MOV A, D
void handle0x7a(Chip* chip, u_int8_t* op) {
    chip->a = chip->d;
    printf("MOV A, D: MOVE VALUE 0x%04X from register D to register A", chip->d);
}

// MOV A, E
void handle0x7b(Chip* chip, u_int8_t* op) {
    chip->a = chip->e;
    printf("MOV A, E: MOVE VALUE 0x%04X from register E to register A", chip->e);
}

// MOV A, H
void handle0x7c(Chip* chip, u_int8_t* op) {
    chip->a = chip->h;
    printf("MOV A, H: MOVE VALUE 0x%04X from register H to register A", chip->h);
}
// MOV A, M
void handle0x7e(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->a = chip->mem[address];
    printf("MOV A, M: SET REGISTER A TO VALUE AT ADDRESS: 0x%04X (0x%04X)", address, chip->a);
}

// ANA A
void handle0xa7(Chip* chip, u_int8_t* op) {
    chip->a &= chip->a;
    printf("ANA A: SETS REGISTER A TO A LOGICAL AND A = A & A (0x%04X)", chip->a);
    setFlags8Bits(chip, chip->a);
    chip->flags->carry = 0;
}

// XRA A
void handle0xaf(Chip* chip, u_int8_t* op) {
    chip->a ^= chip->a;
    printf("XRA A: SETS REGISTER A To  A = A ^ A (0x%04X)", chip->a);
    setFlags8Bits(chip, chip->a);
    chip->flags->carry = 0;
}
// POP B
void handle0xc1(Chip* chip, u_int8_t* op) {
    chip->c = chip->mem[chip->sp];
    chip->b = chip->mem[chip->sp + 1];
    chip->sp += 2;
    printf("POP B: POP FROM STACK. REGISTER C SET TO 0x%04X. REGISTER B SET TO 0x%04X. STACK POINTER -= 2", chip->c, chip->b);
}
// JNZ a16
void handle0xc2(Chip* chip, u_int8_t* op) {
    if (chip->flags->zero == 0) {
        chip->pc = (op[2] << 8) | op[1];
        printf("JNZ a16: SINCE THE ZERO FLAG IS NOT SET, we jump to address: 0x%04X", chip->pc);
    } else {
        printf("JNZ a16: ZERO FLAG IS SET, NO JUMP");
        chip->pc += 2;
    }
}

// JMP a16
void handle0xc3(Chip* chip, u_int8_t* op) {
    chip->pc = (op[2] << 8) | op[1];
    printf("JMP a16: JUMP TO ADDRESS 0x%04X", chip->pc);
}

// PUSH B
void handle0xc5(Chip* chip, u_int8_t* op) {
    chip->mem[chip->sp - 1] = chip->b;
    chip->mem[chip->sp - 2] = chip->c;
    printf("PUSH B: SAVE VALUE AT REGISTER B(0x%04X) and VALUE AT REGISTER C(0x%04X) onto STACK", chip->b, chip->c);
    chip->sp -= 2;
}
// ADI D8
void handle0xc6(Chip* chip, u_int8_t* op) {
    u_int16_t carryCheck = (u_int16_t) chip->a + (u_int16_t) op[1];
    chip->a += op[1];
    printf("ADI D8: ADD IMMEDIATE 0x%04X to Register A to get", chip->a);
    chip->pc += 1;
    setFlags8Bits(chip, chip->a);
    chip->flags->carry = carryCheck > (pow(2, 8) - 1);
}
// RET
void handle0xc9(Chip* chip, u_int8_t* op) {
    chip->pc = (chip->mem[chip->sp + 1] << 8) |chip->mem[chip->sp];
    printf("RET: RETURN TO ADDRESS 0x%04X", chip->pc);
    chip->sp += 2;
}
// CALL
void handle0xcd(Chip* chip, u_int8_t* op) {
    u_int16_t saveOnStack = chip->pc + 2;
    chip->mem[chip->sp - 1] = saveOnStack >> 8;
    chip->mem[chip->sp - 2] = saveOnStack & 0x00ff;
    chip->pc = (op[2] << 8) | op[1];
    chip->sp -= 2;
    printf("CALL: CALL SUBROUTINE AT ADDRESS 0x%04X", chip->pc);
}

// POP D
void handle0xd1(Chip* chip, u_int8_t* op) {
    chip->e = chip->mem[chip->sp];
    chip->d = chip->mem[chip->sp + 1];
    chip->sp += 2;
    printf("POP D: POP FROM STACK. REGISTER E SET TO 0x%04X. REGISTER D SET TO 0x%04X. STACK POINTER -= 2", chip->e, chip->d);
}

// OUT
void handle0xd3(Chip* chip, u_int8_t* op) {
    
}

// PUSH D
void handle0xd5(Chip* chip, u_int8_t* op) {
    chip->mem[chip->sp - 1] = chip->d;
    chip->mem[chip->sp - 2] = chip->e;
    printf("PUSH D: SAVE VALUE AT REGISTER D(0x%04X) and VALUE AT REGISTER E(0x%04X) onto STACK", chip->d, chip->e);
    chip->sp -= 2;
}
// POP H
void handle0xe1(Chip* chip, u_int8_t* op) {
    chip->l = chip->mem[chip->sp];
    chip->h = chip->mem[chip->sp + 1];
    chip->sp += 2;
    printf("POP H: POP FROM STACK. REGISTER L SET TO 0x%04X. REGISTER H SET TO 0x%04X. STACK POINTER -= 2", chip->l, chip->h);
}

// PUSH H
void handle0xe5(Chip* chip, u_int8_t* op) {
    chip->mem[chip->sp - 1] = chip->h;
    chip->mem[chip->sp - 2] = chip->l;
    printf("PUSH H: SAVE VALUE AT REGISTER H(0x%04X) and VALUE AT REGISTER L(0x%04X) onto STACK", chip->h, chip->l);
    chip->sp -= 2;
}
// ANI d8
void handle0xe6(Chip* chip, u_int8_t* op) {
    chip->a &= op[1];
    chip->flags->carry = 0;
    setFlags8Bits(chip, chip->a);
    printf("ANI d8: A = A & immediate(0x%04X)", chip->a);
}

void handle0xeb(Chip* chip, u_int8_t* op) {
    u_int8_t tempE = chip->e;
    u_int8_t tempD = chip->d;
    chip->d = chip->h;
    chip->h = tempD;
    chip->e = chip->l;
    chip->l = tempE;
    printf("SWAP THE VALUES OF HL AND DE. HL = (0x%04X). DE = (0x%04X).", getHL(chip), getDE(chip));

}
// POP PSW
void handle0xf1(Chip* chip, u_int8_t* op) {
    u_int8_t psw = chip->mem[chip->sp];
    chip->a = chip->mem[chip->sp + 1];
    
    chip->sp += 2;
}

void handle0xf5(Chip* chip, u_int8_t* op) {
    
}

void handle0xfb(Chip* chip, u_int8_t* op) {
    chip->flags->interrupt_enabled = 1;
    printf("INTERRUPT ENABLE FlAG SET TO 1");
}

// CPI d8
void handle0xfe(Chip* chip, u_int8_t* op) {
    u_int8_t res = chip->a - op[1];
    chip->flags->carry = res >= 0;
    setFlags8Bits(chip, res);
    chip->pc += 1;
    printf("SUBTRACT IMMEDIATE FROM REGISTER A(0x%04X) AND USE RESULT TO SET FLAGS", res);
}


