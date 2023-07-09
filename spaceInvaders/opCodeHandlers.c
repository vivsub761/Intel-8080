#include "main.h"
#include "helperFunctions.c"
#include "ioDevices.c"


// EACH INSTRUCTION PERFORMS AN OPERATION THEN RETURNS THE AMOUNT OF CYCLES IT TAKES TO COMPLETE THE ACTION

// NOP
int handle0x00(Chip* chip, u_int8_t* op) {
    printf("NO OP");
    return 4;
}
// LXI B,d16
int handle0x01(Chip* chip, u_int8_t* op) {
    printf("LXI B d16: SET REGISTER B to 0x%04X and REGISTER C to 0x%04X", op[2], op[1]);
    chip->b = op[2];
    chip->c = op[1];
    chip->pc += 2;
    return 10;
}
// STAX B
// Set address obtained by combining B and C registers to A in memory
int handle0x02(Chip* chip, u_int8_t* op) {
    u_int16_t address = getBC(chip);
    printf("STAX B: Set memory at address 0x%04X to value in register A: 0x%04X", address, chip->a);
    chip->mem[address] = chip->a;
    return 7;
}
// INX B
// increment BC register pair
int handle0x03(Chip* chip, u_int8_t* op) {
    u_int16_t bc = getBC(chip) + 1;
    printf("INX B: INCREMENT BC RESIGER PAIR TO 0x%04X", bc);
    setBandCtoBC(chip, bc);
    return 5;

}
// INR B
// increment B register
int handle0x04(Chip* chip, u_int8_t* op) {
    printf("INR B: INCREMENT B REGISTER TO 0x%04X", chip->b + 1);
    u_int16_t carryCheck = chip->b + 1;
    chip->b += 1;
    setFlags8Bits(chip, chip->b);
    return 5;
}
// DCR B
// decrement B register
int handle0x05(Chip* chip, u_int8_t* op) {
    printf("DCR B: DECREMENT B REGISTER TO 0x%04X", chip->b - 1);
    chip->b -= 1;
    setFlags8Bits(chip, chip->b);
    return 5;
}
// MVI B,d8
// set register b to immediate
int handle0x06(Chip* chip, u_int8_t* op) {
    printf("MVI B,d8: SET REGSITER B TO 0x%04X", op[1]);
    chip->b = op[1];
    chip->pc += 1;
    return 7;
}
// RLC
// shift a register left by one bit. the leftmost bit is put into the carry flag. The previous value of the carry flag is put in the rightmost bit of the a register
int handle0x07(Chip* chip, u_int8_t* op) {
    
    u_int8_t leftMostBit = chip->a >> 7;
    chip->flags.carry = leftMostBit;
    chip->a = (chip->a << 1) | leftMostBit;
    printf("RLC: SHIFT REGISTER A LEFT ONE BIT. PREVIOUS LEFTMOST BIT PUT IN CARRY FLAG, Leftmost BIt put in rightmost bit, CARRY FLAG: 0x%04X, A RESGISTER: 0x%04X", chip->flags.carry, chip->a);
    return 4;
}

int handle0x08(Chip* chip, u_int8_t* op) {
    printf("NO OP");
    return 4;
}
// DAD B
// HL = HL + BC
int handle0x09(Chip* chip, u_int8_t* op) {
    u_int16_t hl = getHL(chip);
    u_int16_t bc = getBC(chip);
    u_int32_t carryCheck = hl + bc;
    u_int16_t res = hl + bc;
    setHandLtoHL(chip, res);
    printf("DAD B: The value of the HL register pair: 0x%04X is added to the value of the BC Regsiter PAIR: 0x%04X and the result 0x%04X is put into the HL register pair", hl, bc, res);
    chip->flags.carry = carryCheck > (pow(2, 16) - 1);
    return 10;
}
// LDAX B
// Load memory at address stored in BC register into A register
int handle0x0a(Chip* chip, u_int8_t* op) {
    u_int16_t address = getBC(chip);
    chip->a = chip->mem[address];
    printf("LDAX B: Register A set to value 0x%04X at address 0x%04X in memory", chip->a, address);
    return 7;
}

// decrement BC register pair
// DCX B
int handle0x0b(Chip* chip, u_int8_t* op) {
    u_int16_t bc = getBC(chip);
    bc -= 1;
    setBandCtoBC(chip, bc);
    printf("DCX B: Decrement BC register pair to 0x%04X", getBC(chip));
    return 5;
}
// INR C
// Register C += 1;
int handle0x0c(Chip* chip, u_int8_t* op) {
    chip->c += 1;
    setFlags8Bits(chip, chip->c);
    printf("INR C: Increment Register C to 0x%04X", chip->c);
    return 5;
}
// DCR C
// Register C -= 1
int handle0x0d(Chip* chip, u_int8_t* op) {
    chip->c -= 1;
    setFlags8Bits(chip, chip->c);
    printf("DCR C: Decrement Register C to 0x%04X", chip->c);
    return 5;
}

// MVI C, d8
// Move immediate to C register
int handle0x0e(Chip* chip, u_int8_t* op) {
    chip->c = op[1];
    chip->pc += 1;
    printf("MVI C, d8: Move immediate 0x%04X to Register C", op[1]);
    return 7;
}

// RRC
// Rightmost bit of Register A put into carry flag
// register A shifted one bit to right, prevous carry flag put in leftmost bit
int handle0x0f(Chip* chip, u_int8_t* op) {
    chip->flags.carry = chip->a & 1;
    chip->a = (chip->a >> 1) | (chip->flags.carry << 7);
    printf("RRC: SHIFT REGISTER A RIGHT ONE BIT. PREVIOUS RIGHTMOST BIT PUT IN CARRY FLAG, prev Rightmost bit PUT IN LEFTMOST BIT OF A REGISTER, CARRY FLAG: 0x%04X, A RESGISTER: 0x%04X", chip->flags.carry, chip->a);
    return 4;
}
// NOP
int handle0x10(Chip* chip, u_int8_t* op) {
    printf("NOP");
    return 4;
}
// LXI D,d16
// Load 16 bit immediate into DE register pair
int handle0x11(Chip* chip, u_int8_t* op) {
    chip->d = op[2];
    chip->e = op[1];
    chip->pc += 2;
    printf("LXI D,d16: Load 16 bit immediate into DE register pair. Register D = 0x%04X, Register E = 0x%04X", op[2], op[1]);
    return 10;
}
// STAX D
// Store contents of register A in memory address at DE register pair
int handle0x12(Chip* chip, u_int8_t* op) {
    u_int16_t address = getDE(chip);
    chip->mem[address] = chip->a;
    printf("STAX D: Set memory at address 0x%04X to value in register A: 0x%04X", address, chip->a);
    return 7;
}

// INX D
// Increment DE register Pair
int handle0x13(Chip* chip, u_int8_t* op) {
    u_int16_t de = getDE(chip) + 1;
    printf("INX D: INCREMENT DE RESIGER PAIR TO 0x%04X", de);
    setDandEtoDE(chip, de);
    return 5;
}

// INR D
// increment D register
int handle0x14(Chip* chip, u_int8_t* op) {
    printf("INR D: INCREMENT D REGISTER TO 0x%04X", chip->d + 1);
    chip->d += 1;
    setFlags8Bits(chip, chip->d);
    return 5;
}

// DCR D
// decrement D register
int handle0x15(Chip* chip, u_int8_t* op) {
    printf("DCR D: DECREMENT D REGISTER TO 0x%04X", chip->d - 1);
    chip->d -= 1;
    setFlags8Bits(chip, chip->d);
    return 5;
}

// MVI D,d8
// set register d to immediate
int handle0x16(Chip* chip, u_int8_t* op) {
    printf("MVI D,d8: SET REGSITER D TO 0x%04X", op[1]);
    chip->d = op[1];
    chip->pc += 1;
    return 7;
}
// RAL
int handle0x17(Chip* chip, u_int8_t* op) {
    u_int8_t carryFlag = chip->flags.carry;
    chip->flags.carry = chip->a >> 7;
    chip->a = (chip->a << 1) | carryFlag;
    printf("RLC: SHIFT REGISTER A LEFT ONE BIT. PREVIOUS LEFTMOST BIT PUT IN CARRY FLAG, Previous carry bit put in rightmost bit, CARRY FLAG: 0x%04X, A RESGISTER: 0x%04X", chip->flags.carry, chip->a);
    return 4;
}

int handle0x18(Chip* chip, u_int8_t* op) {
    printf("NOP");
    return 4;
}

// DAD D
// HL = HL + DE
int handle0x19(Chip* chip, u_int8_t* op) {
    u_int16_t hl = getHL(chip);
    u_int16_t de = getDE(chip);
    u_int32_t carryCheck = hl + de;
    u_int16_t res = hl + de;
    setHandLtoHL(chip, res);
    printf("DAD D: The value of the HL register pair: 0x%04X is added to the value of the DE Regsiter PAIR: 0x%04X and the result 0x%04X is put into the HL register pair", hl, de, res);
    chip->flags.carry = carryCheck > (pow(2, 16) - 1);
    return 10;
}

// LDAX D
// Load memory at address stored in DE register into A register
int handle0x1a(Chip* chip, u_int8_t* op) {
    u_int16_t address = getDE(chip);
    chip->a = chip->mem[address];
    printf("LDAX D: Register A set to value 0x%04X at address 0x%04X in memory", chip->a, address);
    return 7;
}

// decrement DE register pair
// DCX D
int handle0x1b(Chip* chip, u_int8_t* op) {
    u_int16_t de = getDE(chip);
    de -= 1;
    setBandCtoBC(chip, de);
    printf("DCX D: Decrement BC register pair to 0x%04X", getDE(chip));
    return 5;
}

// INR E
// Register E += 1;
int handle0x1c(Chip* chip, u_int8_t* op) {
    chip->e += 1;
    setFlags8Bits(chip, chip->e);
    printf("INR E: Increment Register E to 0x%04X", chip->e);
    return 5;
}

// DCR E
// Register E -= 1
int handle0x1d(Chip* chip, u_int8_t* op) {
    chip->e -= 1;
    setFlags8Bits(chip, chip->e);
    printf("DCR E: Decrement Register E to 0x%04X", chip->e);
    return 5;
}

// MVI E, d8
// Move immediate to E register
int handle0x1e(Chip* chip, u_int8_t* op) {
    chip->e = op[1];
    chip->pc += 1;
    printf("MVI E, d8: Move immediate 0x%04X to Register E", op[1]);
    return 7;
}
// RAR
int handle0x1f(Chip* chip, u_int8_t* op) {
    u_int8_t carryFlag = chip->flags.carry;
    chip->flags.carry = chip->a & 1;
    chip->a = (chip->a >> 1) | (carryFlag << 7);
    printf("SHIFT REGISTER A ONE BIT TO THE RIGHT, PREV RIGHTMOST BIT PUT INTO CARRY FLAG, PREV CARRY FLAG PUT INTO LEFTMOST BIT. Register A: 0x%04X, Carry FLAG: 0x%04X ", chip->a, chip->flags.carry);
    return 4;
}
// NOP
int handle0x20(Chip* chip, u_int8_t* op) {
    printf("NOP");
    return 4;
}

// LXI H,d16
// Load 16 bit immediate into HL register pair
int handle0x21(Chip* chip, u_int8_t* op) {
    chip->h = op[2];
    chip->l = op[1];
    chip->pc += 2;
    printf("LXI H,d16: Load 16 bit immediate into HL register pair. Register H = 0x%04X, Register L = 0x%04X", op[2], op[1]);
    return 10;
}

// SHLD a16
// save register L at address a16 and register H at address a16 + 1
int handle0x22(Chip* chip, u_int8_t* op) {
    u_int16_t address = (op[2] << 8) | op[1];
    chip->mem[address] = chip->l;
    chip->mem[address + 1] = chip->h;
    printf("SAVING REGISTER L: 0x%04X at address 0x%04X and REGISTER H: 0x%04X at address 0x%04X", chip->l, address, chip->h, address + 1);
    return 16;
}

// INX H
// Increment HL register Pair
int handle0x23(Chip* chip, u_int8_t* op) {
    u_int16_t hl = getHL(chip) + 1;
    printf("INX H: INCREMENT HL RESIGER PAIR TO 0x%04X", hl);
    setHandLtoHL(chip, hl);
    return 5;
}

// INR H
// increment H register
int handle0x24(Chip* chip, u_int8_t* op) {
    printf("INR H: INCREMENT H REGISTER TO 0x%04X", chip->h + 1);
    chip->h += 1;
    setFlags8Bits(chip, chip->h);
    return 5;
}

// DCR H
// decrement H register
int handle0x25(Chip* chip, u_int8_t* op) {
    printf("DCR H: DECREMENT H REGISTER TO 0x%04X", chip->h - 1);
    chip->h -= 1;
    setFlags8Bits(chip, chip->h);
    return 5;
}

// MVI H,d8
// set register H to immediate
int handle0x26(Chip* chip, u_int8_t* op) {
    printf("MVI H,d8: SET REGSITER H TO 0x%04X", op[1]);
    chip->h = op[1];
    chip->pc += 1;
    return 7;
}

// DAA
int handle0x27(Chip* chip, u_int8_t* op) {
    printf("DAD: STILL UNIMPLEMENTED");
    return 4;
}

int handle0x28(Chip* chip, u_int8_t* op) {
    printf("NOP");
    return 4;
}

// DAD H
// HL = HL + DE
int handle0x29(Chip* chip, u_int8_t* op) {
    u_int16_t hl = getHL(chip);
    u_int32_t carryCheck = hl + hl;
    u_int16_t res = hl + hl;
    setHandLtoHL(chip, res);
    printf("DAD H: The value of the HL register pair: 0x%04X is added to itself and the result 0x%04X is put into the HL register pair", hl, res);
    chip->flags.carry = carryCheck > (pow(2, 16) - 1);
    return 10;
}

// LHLD a16
int handle0x2a(Chip* chip, u_int8_t* op) {
    u_int16_t address = (op[2] << 8) | op[1];
    chip->l = chip->mem[address];
    chip->h = chip->mem[address + 1];
    printf("REGISTER L: 0x%04X SET TO value at address 0x%04X, REGISTER H: 0x%04X SET TO value at address 0x%04X", chip->l, address, chip->h, address + 1);
    return 16;
}   

// decrement HL register pair
// DCX H
int handle0x2b(Chip* chip, u_int8_t* op) {
    u_int16_t hl = getHL(chip);
    hl -= 1;
    setBandCtoBC(chip, hl);
    printf("DCX H: Decrement HL register pair to 0x%04X", getHL(chip));
    return 5;
}

// INR L
// Register L += 1;
int handle0x2c(Chip* chip, u_int8_t* op) {
    chip->l += 1;
    setFlags8Bits(chip, chip->l);
    printf("INR L: Increment Register L to 0x%04X", chip->l);
    return 5;
}

// DCR L
// decrement L register
int handle0x2d(Chip* chip, u_int8_t* op) {
    printf("DCR L: DECREMENT L REGISTER TO 0x%04X", chip->l - 1);
    chip->l -= 1;
    setFlags8Bits(chip, chip->l);
    return 5;
}

// MVI L, d8
// Move immediate to L register
int handle0x2e(Chip* chip, u_int8_t* op) {
    chip->l = op[1];
    chip->pc += 1;
    printf("MVI L, d8: Move immediate 0x%04X to Register L", op[1]);
    return 7;
}
// CMA
int handle0x2f(Chip* chip, u_int8_t* op) {
    chip->a = ~(chip->a);
    printf("CMA: INVERT the bits in Register A: 0x%04X", chip->a);
    return 4;
}
// NOP
int handle0x30(Chip* chip, u_int8_t* op) {
    printf("NOP");
    return 4;
}
// LXI SP,d16
int handle0x31(Chip* chip, u_int8_t* op) {
    chip->sp = (op[2] << 8) | op[1];
    printf("LXI SP,d16: change stack pointer to immediate 0x%04X", chip->sp);
    chip->pc += 2;
    return 10;
}
// STA a16
int handle0x32(Chip* chip, u_int8_t* op) {
    u_int16_t address = (op[2] << 8) | op[1];
    printf("STA a16: Store the value in register A: 0x%04X at address 0x%04X", chip->a, address);
    chip->mem[address] = chip->a;
    chip->pc += 2;
    return 13;
}

// MVI M,d8
// Move the immediate to the address the HL pair points to 
int handle0x36(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    printf("MVI B,d8: SET Memory at address 0x%04X TO 0x%04X", address, op[1]);
    chip->mem[address] = op[1];
    chip->pc += 1;
    return 10;
}
// LDA a16
int handle0x3a(Chip* chip, u_int8_t* op) {
    u_int16_t address = (op[2] << 8) | op[1];
    chip->a = chip->mem[address];
    printf("LDA a16: LOAD VALUE(0x%04X) from address 0x%04X into Register A", address, chip->a); 
    chip->pc += 2;
    return 13;
}

// MVI A, d8
// Move immediate to A register
int handle0x3e(Chip* chip, u_int8_t* op) {
    chip->a = op[1];
    chip->pc += 1;
    printf("MVI A, d8: Move immediate 0x%04X to Register A", op[1]);
    return 7;
}
// MOV D,M
// Set Register D to value pointed at by HL register
int handle0x56(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->d = chip->mem[address];
    printf("MOV D,M: SET REGISTER D(0x%04X) to value at address 0x%04X", chip->d, address);
    return 7;
}
// MOV E,M
// Set Register E to value pointed at by HL register
int handle0x5e(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->e = chip->mem[address];
    printf("MOV E,M: SET REGISTER E(0x%04X) to value at address 0x%04X", chip->e, address);
    return 7;
}

// MOV H,M
// Set Register E to value pointed at by HL register
int handle0x66(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->h = chip->mem[address];
    printf("MOV H,M: SET REGISTER H(0x%04X) to value at address 0x%04X", chip->h, address);
    return 7;
}
// MOV L, A
int handle0x6f(Chip* chip, u_int8_t* op) {
    chip->a = chip->l;
    printf("MOV L, A: MOVE VALUE FROM REGISTER L(0x%04X) to register A",chip->a);
    return 5;
}
// 	MOV M,A
int handle0x77(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->mem[address] = chip->a;
    printf("MOV M, A: SET VALUE AT ADDRESS 0x%04X to register A: (0x%04X)", address, chip->a);
    return 7;
}

// MOV A, D
int handle0x7a(Chip* chip, u_int8_t* op) {
    chip->a = chip->d;
    printf("MOV A, D: MOVE VALUE 0x%04X from register D to register A", chip->d);
    return 5;
}

// MOV A, E
int handle0x7b(Chip* chip, u_int8_t* op) {
    chip->a = chip->e;
    printf("MOV A, E: MOVE VALUE 0x%04X from register E to register A", chip->e);
    return 5;
}

// MOV A, H
int handle0x7c(Chip* chip, u_int8_t* op) {
    chip->a = chip->h;
    printf("MOV A, H: MOVE VALUE 0x%04X from register H to register A", chip->h);
    return 5;
}
// MOV A, M
int handle0x7e(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->a = chip->mem[address];
    printf("MOV A, M: SET REGISTER A TO VALUE AT ADDRESS: 0x%04X (0x%04X)", address, chip->a);
    return 7;
}

// ANA A
int handle0xa7(Chip* chip, u_int8_t* op) {
    chip->a &= chip->a;
    printf("ANA A: SETS REGISTER A TO A LOGICAL AND A = A & A (0x%04X)", chip->a);
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    return 4;
}

// XRA A
int handle0xaf(Chip* chip, u_int8_t* op) {
    chip->a ^= chip->a;
    printf("XRA A: SETS REGISTER A To  A = A ^ A (0x%04X)", chip->a);
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    return 4;
}
// POP B
int handle0xc1(Chip* chip, u_int8_t* op) {
    chip->c = chip->mem[chip->sp];
    chip->b = chip->mem[chip->sp + 1];
    chip->sp += 2;
    printf("POP B: POP FROM STACK. REGISTER C SET TO 0x%04X. REGISTER B SET TO 0x%04X. STACK POINTER -= 2", chip->c, chip->b);
    return 10;
}
// JNZ a16
int handle0xc2(Chip* chip, u_int8_t* op) {
    if (chip->flags.zero == 0) {
        chip->pc = (op[2] << 8) | op[1];
        printf("JNZ a16: SINCE THE ZERO FLAG IS NOT SET, we jump to address: 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        printf("JNZ a16: ZERO FLAG IS SET, NO JUMP");
        chip->pc += 2;
    }
    return 10;
}

// JMP a16
int handle0xc3(Chip* chip, u_int8_t* op) {
    chip->pc = (op[2] << 8) | op[1];
    printf("JMP a16: JUMP TO ADDRESS 0x%04X", chip->pc);
    chip->pc -= 1;
    return 10;
}

// PUSH B
int handle0xc5(Chip* chip, u_int8_t* op) {
    chip->mem[chip->sp - 1] = chip->b;
    chip->mem[chip->sp - 2] = chip->c;
    printf("PUSH B: SAVE VALUE AT REGISTER B(0x%04X) and VALUE AT REGISTER C(0x%04X) onto STACK", chip->b, chip->c);
    chip->sp -= 2;
    return 11;
}
// ADI D8
int handle0xc6(Chip* chip, u_int8_t* op) {
    u_int16_t carryCheck = (u_int16_t) chip->a + (u_int16_t) op[1];
    chip->a += op[1];
    printf("ADI D8: ADD IMMEDIATE 0x%04X to Register A to get", chip->a);
    chip->pc += 1;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = carryCheck > (pow(2, 8) - 1);
    return 7;
}
// RET
int handle0xc9(Chip* chip, u_int8_t* op) {
    chip->pc = (chip->mem[chip->sp + 1] << 8) |chip->mem[chip->sp];
    printf("RET: RETURN TO ADDRESS 0x%04X", chip->pc);
    chip->sp += 2;
    return 10;
}
// CALL
int handle0xcd(Chip* chip, u_int8_t* op) {
    u_int16_t saveOnStack = chip->pc + 2;
    chip->mem[chip->sp - 1] = saveOnStack >> 8;
    chip->mem[chip->sp - 2] = saveOnStack & 0x00ff;
    chip->pc = (op[2] << 8) | op[1];
    chip->sp -= 2;
    printf("CALL: CALL SUBROUTINE AT ADDRESS 0x%04X", chip->pc);
    chip->pc -= 1;
    return 17;
}

// POP D
int handle0xd1(Chip* chip, u_int8_t* op) {
    chip->e = chip->mem[chip->sp];
    chip->d = chip->mem[chip->sp + 1];
    chip->sp += 2;
    printf("POP D: POP FROM STACK. REGISTER E SET TO 0x%04X. REGISTER D SET TO 0x%04X. STACK POINTER -= 2", chip->e, chip->d);
    return 10;
}

// OUT
int handle0xd3(Chip* chip, u_int8_t* op) {
    deviceOut(chip, op[1], chip->a);
    printf("OUT: Write value in REGISTER A: 0x%04X to I/O Port %d", chip->a, op[1]);
    chip->pc += 1;
    return 10;
}

// PUSH D
int handle0xd5(Chip* chip, u_int8_t* op) {
    chip->mem[chip->sp - 1] = chip->d;
    chip->mem[chip->sp - 2] = chip->e;
    printf("PUSH D: SAVE VALUE AT REGISTER D(0x%04X) and VALUE AT REGISTER E(0x%04X) onto STACK", chip->d, chip->e);
    chip->sp -= 2;
    return 11;
}
// IN
int handle0xdb(Chip* chip, u_int8_t* op) {
    deviceIn(chip, op[1]);
    printf("IN: Data from Port %d written to Register A: 0x%04X", op[1], chip->a);
    chip->pc += 1;
    return 10;

}
// POP H
int handle0xe1(Chip* chip, u_int8_t* op) {
    chip->l = chip->mem[chip->sp];
    chip->h = chip->mem[chip->sp + 1];
    chip->sp += 2;
    printf("POP H: POP FROM STACK. REGISTER L SET TO 0x%04X. REGISTER H SET TO 0x%04X. STACK POINTER -= 2", chip->l, chip->h);
    return 10;
}

// PUSH H
int handle0xe5(Chip* chip, u_int8_t* op) {
    chip->mem[chip->sp - 1] = chip->h;
    chip->mem[chip->sp - 2] = chip->l;
    printf("PUSH H: SAVE VALUE AT REGISTER H(0x%04X) and VALUE AT REGISTER L(0x%04X) onto STACK", chip->h, chip->l);
    chip->sp -= 2;
    return 11;
}
// ANI d8
int handle0xe6(Chip* chip, u_int8_t* op) {
    chip->a &= op[1];
    chip->flags.carry = 0;
    setFlags8Bits(chip, chip->a);
    printf("ANI d8: A = A & immediate(0x%04X)", chip->a);
    return 7;
}
// XCHG
int handle0xeb(Chip* chip, u_int8_t* op) {
    u_int8_t tempE = chip->e;
    u_int8_t tempD = chip->d;
    chip->d = chip->h;
    chip->h = tempD;
    chip->e = chip->l;
    chip->l = tempE;
    printf("XCHG: SWAP THE VALUES OF HL AND DE. HL = (0x%04X). DE = (0x%04X).", getHL(chip), getDE(chip));
    return 5;

}
// POP PSW
int handle0xf1(Chip* chip, u_int8_t* op) {
    u_int8_t psw = chip->mem[chip->sp];
    chip->a = chip->mem[chip->sp + 1];
    chip->flags.carry = (0x05 == (psw & 0x08));
    chip->flags.parity = (0x04 == (psw & 0x04));
    chip->flags.sign = (0x02 == (psw & 0x02));
    chip->flags.zero = (0x01 == (psw & 0x01));
    chip->sp += 2;
    return 10;
}
// PUSH PSW
int handle0xf5(Chip* chip, u_int8_t* op) {
    chip->mem[chip->sp - 1] = chip->a;
    u_int8_t psw = 2;
    psw |= chip->flags.carry;
    psw |= (chip->flags.parity << 2);
    psw |= (chip->flags.auxCarry << 4);
    psw |= (chip->flags.zero << 6);
    psw |= (chip->flags.sign << 7);
    chip->mem[chip->sp - 2] = psw;
    chip->sp -= 2;
    return 11;
}
// EI
int handle0xfb(Chip* chip, u_int8_t* op) {
    chip->flags.interrupt_enabled = 1;
    printf("INTERRUPT ENABLE FlAG SET TO 1");
    return 4;
}

// CPI d8
int handle0xfe(Chip* chip, u_int8_t* op) {
    u_int8_t res = chip->a - op[1];
    chip->flags.carry = 0;
    setFlags8Bits(chip, res);
    chip->pc += 1;
    printf("SUBTRACT IMMEDIATE FROM REGISTER A(0x%04X) AND USE RESULT TO SET FLAGS", res);
    return 7;
}


