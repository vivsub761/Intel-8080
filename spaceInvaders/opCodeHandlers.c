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
    printf("INX B: INCREMENT BC REGISTER PAIR TO 0x%04X", bc);
    setBandCtoBC(chip, bc);
    return 5;

}
// INR B
// increment B register
int handle0x04(Chip* chip, u_int8_t* op) {
    printf("INR B: INCREMENT B REGISTER TO 0x%04X", chip->b + 1);
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
    u_int32_t carryCheck = (u_int32_t) hl + (u_int32_t) de;
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
    u_int16_t de = getDE(chip) - 1;
    setDandEtoDE(chip, de);
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
    printf("SHLD: SAVING REGISTER L: 0x%04X at address 0x%04X and REGISTER H: 0x%04X at address 0x%04X", chip->l, address, chip->h, address + 1);
    chip->pc += 2;
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

// DAD
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
    u_int32_t carryCheck = (u_int32_t) hl + (u_int32_t) hl;
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
    printf("LHLD: REGISTER L: 0x%04X SET TO value at address 0x%04X, REGISTER H: 0x%04X SET TO value at address 0x%04X", chip->l, address, chip->h, address + 1);
    chip->pc += 2;
    return 16;
}   

// decrement HL register pair
// DCX H
int handle0x2b(Chip* chip, u_int8_t* op) {
    u_int16_t hl = getHL(chip);
    hl -= 1;
    setHandLtoHL(chip, hl);
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
    chip->a = ~chip->a;
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

// INX SP
int handle0x33(Chip* chip, u_int8_t* op) {
    chip->sp += 1;
    printf("INCREMENT STACK POINTER TO 0x%04X", chip->sp);
    return 5;
}

// INR M
int handle0x34(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->mem[address] += 1;
    printf("INCREMENT MEMORY AT ADDRESS 0x%04X to get 0x%04X", address, chip->mem[address]);
    setFlags8Bits(chip, chip->mem[address]);
}
// DCR M
int handle0x35(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->mem[address] -= 1;
    printf("DECREMENT MEMORY AT ADDRESS 0x%04X to get 0x%04X", address, chip->mem[address]);
    setFlags8Bits(chip, chip->mem[address]);
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
// STC, BYTES 1, CYCLES 4, FLAGS: CARRY
int handle0x37(Chip* chip, u_int8_t* op) {
    chip->flags.carry = 1;
    printf("SET CARRY FLAG to 1");
    return 4;
}
// NOP
int handle0x38(Chip* chip, u_int8_t* op) {
    printf("NOP");
    return 4;
}

// DAD SP, BYTES 1, CYCLES 10, FLAGS: CARRY
int handle0x39(Chip* chip, u_int8_t* op) {
    u_int16_t hl = getHL(chip);
    chip->flags.carry = ((u_int32_t) hl + (u_int32_t) chip->sp) > (pow(2, 16) - 1);
    u_int16_t res = hl + chip->sp;
    setHandLtoHL(chip, res);
    printf("ADD HL REGISTER TO STACK POINTER AND STORE RESULT 0x%04X in HL register", getHL(chip));
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

// DCX SP, BYTES 1  CYCLES 5, FLAGS NONE
int handle0x3b(Chip* chip, u_int8_t* op) {
    chip->sp -= 1;
    printf("DECREMENT STACK POINTER");
    return 5;
}
// INR A, BYTES 1, CYCLES 5, FLAGS: SZAP
int handle0x3c(Chip* chip, u_int8_t* op) {
    chip->a += 1;
    setFlags8Bits(chip, chip->a);
    printf("INCREMENT REGISTER A TO 0x%04X", chip->a);
    return 5;
}

// DCR A, BYTES 1, CYCLES 5, FLAGS: SZAP
int handle0x3d(Chip* chip, u_int8_t* op) {
    chip->a -= 1;
    setFlags8Bits(chip, chip->a);
    printf("DECREMENT REGISTER A TO 0x%04X", chip->a);
    return 5;
}


// MVI A, d8
// Move immediate to A register
int handle0x3e(Chip* chip, u_int8_t* op) {
    chip->a = op[1];
    chip->pc += 1;
    printf("MVI A, d8: Move immediate 0x%04X to Register A", op[1]);
    return 7;
}

// CMC, BYTES 1, CYCLES 4, FLAGS: C
int handle0x3f(Chip* chip, u_int8_t* op) {
    chip->flags.carry = chip->flags.carry == 1 ? 0 : 1;
    printf("SWITCH CARRY FLAG TO 0x%04X", chip->flags.carry);
    return 4;
}
// MOV B, B
int handle0x40(Chip* chip, u_int8_t* op) {
    chip->b = chip->b;
    printf("NOP");
    return 5;
}
// MOV B, C
int handle0x41(Chip* chip, u_int8_t* op) {
    chip->b = chip->c;
    printf("MOV B, C: MOVE VALUE 0x%04X from Register C to B", chip->b);
    return 5;
}

int handle0x42(Chip* chip, u_int8_t* op) {
    chip->b = chip->d;
    printf("MOV B, D: MOVE VALUE 0x%04X from Register D to B", chip->b);
    return 5;
}

int handle0x43(Chip* chip, u_int8_t* op) {
    chip->b = chip->e;
    printf("MOV B, E: MOVE VALUE 0x%04X from Register E to B", chip->b);
    return 5;
}

int handle0x44(Chip* chip, u_int8_t* op) {
    chip->b = chip->h;
    printf("MOV B, H: MOVE VALUE 0x%04X from Register H to B", chip->b);
    return 5;
}

int handle0x45(Chip* chip, u_int8_t* op) {
    chip->b = chip->l;
    printf("MOV B, L: MOVE VALUE 0x%04X from Register L to B", chip->b);
    return 5;
}

int handle0x46(Chip* chip, u_int8_t* op) {
    chip->b = chip->mem[getHL(chip)];
    printf("MOV B, M: SET REGISTER B TO VALUE(0x%04X) AT MEMORY ADDRESS 0x%04X", chip->b, getHL(chip));
    return 7;
}

int handle0x47(Chip* chip, u_int8_t* op) {
   chip->b = chip->a;
   printf("MOV B A: MOVE VALUE 0x%04X from Register A to B", chip->b);
   return 5;
}
// MOV C, B
int handle0x48(Chip* chip, u_int8_t* op) {
    chip->c = chip->b;
    printf("MOV C B: MOVE VALUE 0x%04X from Register B to C", chip->b);
    return 5;
}
// MOV C, C
int handle0x49(Chip* chip, u_int8_t* op) {
    chip->c = chip->c;
    printf("MOV C, C (NO OP)");
    return 5;
}

int handle0x4a(Chip* chip, u_int8_t* op) {
    chip->c = chip->d;
    printf("MOV C D: MOVE VALUE 0x%04X from Register D to C", chip->b);
    return 5;
}

int handle0x4b(Chip* chip, u_int8_t* op) {
    chip->c = chip->e;
    printf("MOV C E: MOVE VALUE 0x%04X from Register E to C", chip->b);
    return 5;
}

int handle0x4c(Chip* chip, u_int8_t* op) {
    chip->c = chip->h;
    printf("MOV C H: MOVE VALUE 0x%04X from Register H to C", chip->b);
    return 5;
}

int handle0x4d(Chip* chip, u_int8_t* op) {
    chip->c = chip->l;
    printf("MOV C L: MOVE VALUE 0x%04X from Register L to C", chip->b);
    return 5;
}

int handle0x4e(Chip* chip, u_int8_t* op) {
    chip->c = chip->mem[getHL(chip)];
    printf("MOV C, M: SET REGISTER C TO VALUE(0x%04X) AT MEMORY ADDRESS 0x%04X", chip->c, getHL(chip));
    return 7;
}

int handle0x4f(Chip* chip, u_int8_t* op) {
    chip->c = chip->a;
    printf("MOV C A: MOVE VALUE 0x%04X from Register A to C", chip->b);
    return 5;
}

int handle0x50(Chip* chip, u_int8_t* op) {
    chip->d = chip->b;
    printf("MOV D B: MOVE VALUE 0x%04X from Register B to D", chip->d);
    return 5;
}

int handle0x51(Chip* chip, u_int8_t* op) {
    chip->d = chip->c;
    printf("MOV D C: MOVE VALUE 0x%04X from Register C to D", chip->d);
    return 5;
}

int handle0x52(Chip* chip, u_int8_t* op) {
    chip->d = chip->d;
    printf("MOV D, D: MOVE D TO D (NO OP)");
    return 5;
}

int handle0x53(Chip* chip, u_int8_t* op) {
    chip->d = chip->e;
    printf("MOV D E: MOVE VALUE 0x%04X from Register E to D", chip->d);
    return 5;
}

int handle0x54(Chip* chip, u_int8_t* op) {
    chip->d = chip->h;
    printf("MOV D H: MOVE VALUE 0x%04X from Register H to D", chip->d);
    return 5;
}

int handle0x55(Chip* chip, u_int8_t* op) {
    chip->d = chip->l;
    printf("MOV D L: MOVE VALUE 0x%04X from Register L to D", chip->d);
    return 5;
}

// MOV D,M
// Set Register D to value pointed at by HL register
int handle0x56(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->d = chip->mem[address];
    printf("MOV D,M: SET REGISTER D(0x%04X) to value at address 0x%04X", chip->d, address);
    return 7;
}

int handle0x57(Chip* chip, u_int8_t* op) {
    chip->d = chip->a;
    printf("MOV D A: MOVE VALUE 0x%04X from Register A to D", chip->d);
    return 5;
}
// b c d e h l m a
int handle0x58(Chip* chip, u_int8_t* op) {
    chip->e = chip->b;
    printf("MOV E B: MOVE VALUE 0x%04X from Register B to E", chip->e);
    return 5;
}

int handle0x59(Chip* chip, u_int8_t* op) {
    chip->e = chip->c;
    printf("MOV E C: MOVE VALUE 0x%04X from Register C to E", chip->e);
    return 5;
}

int handle0x5a(Chip* chip, u_int8_t* op) {
    chip->e = chip->d;
    printf("MOV E D: MOVE VALUE 0x%04X from Register D to E", chip->e);
    return 5;
}

int handle0x5b(Chip* chip, u_int8_t* op) {
    chip->e = chip->e;
    printf("MOV E, E: NO OP)");
    return 5;
}

int handle0x5c(Chip* chip, u_int8_t* op) {
    chip->e = chip->h;
    printf("MOV E B: MOVE VALUE 0x%04X from Register H to E", chip->e);
    return 5;
}

int handle0x5d(Chip* chip, u_int8_t* op) {
    chip->e = chip->l;
    printf("MOV E L: MOVE VALUE 0x%04X from Register L to E", chip->e);
    return 5;
}
// MOV E,M
// Set Register E to value pointed at by HL register
int handle0x5e(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->e = chip->mem[address];
    printf("MOV E,M: SET REGISTER E(0x%04X) to value at address 0x%04X", chip->e, address);
    return 7;
}

int handle0x5f(Chip* chip, u_int8_t* op) {
    chip->e = chip->a;
    printf("MOV E A: MOVE VALUE 0x%04X from Register A to E", chip->e);
    return 5;
}
// b c d e h l m a
int handle0x60(Chip* chip, u_int8_t* op) {
    chip->h = chip->b;
    printf("MOV H B: MOVE VALUE 0x%04X from Register B to H", chip->h);
    return 5;
}

int handle0x61(Chip* chip, u_int8_t* op) {
    chip->h = chip->c;
    printf("MOV H C: MOVE VALUE 0x%04X from Register C to H", chip->h);
    return 5;
}

int handle0x62(Chip* chip, u_int8_t* op) {
    chip->h = chip->d;
    printf("MOV H D: MOVE VALUE 0x%04X from Register D to H", chip->h);
    return 5;
}

int handle0x63(Chip* chip, u_int8_t* op) {
    chip->h = chip->e;
    printf("MOV H E: MOVE VALUE 0x%04X from Register E to H", chip->h);
    return 5;
}

int handle0x64(Chip* chip, u_int8_t* op) {
    chip->h = chip->h;
    printf("MOV H H: NO OP");
    return 5;
}

int handle0x65(Chip* chip, u_int8_t* op) {
    chip->h = chip->l;
    printf("MOV H L: MOVE VALUE 0x%04X from Register L to H", chip->h);
    return 5;
}

// MOV H,M
// Set Register E to value pointed at by HL register
int handle0x66(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->h = chip->mem[address];
    printf("MOV H,M: SET REGISTER H(0x%04X) to value at address 0x%04X", chip->h, address);
    return 7;
}

int handle0x67(Chip* chip, u_int8_t* op) {
    chip->h = chip->a;
    printf("MOV H A: MOVE VALUE 0x%04X from Register A to H", chip->h);
    return 5;
}
// b c d e h l m a
int handle0x68(Chip* chip, u_int8_t* op) {
    chip->l = chip->b;
    printf("MOV L B: MOVE VALUE 0x%04X from Register B to L", chip->l);
    return 5;
}

int handle0x69(Chip* chip, u_int8_t* op) {
    chip->l = chip->c;
    printf("MOV L C: MOVE VALUE 0x%04X from Register C to L", chip->l);
    return 5;
}

int handle0x6a(Chip* chip, u_int8_t* op) {
    chip->l = chip->d;
    printf("MOV L D: MOVE VALUE 0x%04X from Register D to L", chip->l);
    return 5;
}

int handle0x6b(Chip* chip, u_int8_t* op) {
    chip->l = chip->e;
    printf("MOV L E: MOVE VALUE 0x%04X from Register E to L", chip->l);
    return 5;
}

int handle0x6c(Chip* chip, u_int8_t* op) {
    chip->l = chip->h;
    printf("MOV L H: MOVE VALUE 0x%04X from Register H to L", chip->l);
    return 5;
}

int handle0x6d(Chip* chip, u_int8_t* op) {
    chip->l = chip->l;
    printf("MOV L, L: NO OP");
}

int handle0x6e(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->l = chip->mem[address];
    printf("MOV L,M: SET REGISTER L(0x%04X) to value at address 0x%04X", chip->l, address);
    return 7;
}
// MOV L, A
int handle0x6f(Chip* chip, u_int8_t* op) {
    chip->l = chip->a;
    printf("MOV L, A: MOVE VALUE FROM REGISTER A(0x%04X) to register L",chip->a);
    return 5;
}

int handle0x70(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->mem[address] = chip->b;
    printf("MOV M, B: SET VALUE AT ADDRESS 0x%04X to register B: (0x%04X)", address, chip->b);
    return 7;
}

int handle0x71(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->mem[address] = chip->c;
    printf("MOV M, C: SET VALUE AT ADDRESS 0x%04X to register C: (0x%04X)", address, chip->c);
    return 7;
}

int handle0x72(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->mem[address] = chip->d;
    printf("MOV M, D: SET VALUE AT ADDRESS 0x%04X to register D: (0x%04X)", address, chip->d);
    return 7;
}

int handle0x73(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->mem[address] = chip->e;
    printf("MOV M, E: SET VALUE AT ADDRESS 0x%04X to register A: (0x%04X)", address, chip->e);
    return 7;
}

int handle0x74(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->mem[address] = chip->h;
    printf("MOV M, H: SET VALUE AT ADDRESS 0x%04X to register H: (0x%04X)", address, chip->h);
    return 7;
}

int handle0x75(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->mem[address] = chip->l;
    printf("MOV M, L: SET VALUE AT ADDRESS 0x%04X to register L: (0x%04X)", address, chip->l);
    return 7;
}

int handle0x76(Chip* chip, u_int8_t* op) {
    printf("hALT");
    return 7;
}
// 	MOV M,A
int handle0x77(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->mem[address] = chip->a;
    printf("MOV M, A: SET VALUE AT ADDRESS 0x%04X to register A: (0x%04X)", address, chip->a);
    return 7;
}
// bcdehlma
int handle0x78(Chip* chip, u_int8_t* op) {
    chip->a = chip->b;
    printf("MOV A B: MOVE VALUE 0x%04X from Register B to A", chip->a);
    return 5;
}

int handle0x79(Chip* chip, u_int8_t* op) {
    chip->a = chip->c;
    printf("MOV A C: MOVE VALUE 0x%04X from Register C to A", chip->a);
    return 5;
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

int handle0x7d(Chip* chip, u_int8_t* op) {
    chip->a = chip->l;
    printf("MOV A L: MOVE VALUE 0x%04X from Register L to A", chip->l);
    return 5;
}

// MOV A, M
int handle0x7e(Chip* chip, u_int8_t* op) {
    u_int16_t address = getHL(chip);
    chip->a = chip->mem[address];
    printf("MOV A, M: SET REGISTER A TO VALUE AT ADDRESS: 0x%04X (0x%04X)", address, chip->a);
    return 7;
}

int handle0x7f(Chip* chip, u_int8_t* op) {
    chip->a = chip->a;
    printf("MOV A, A: NO OP");
    return 5;
}

int handle0x80(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) chip->b;
    chip->flags.carry = res > (pow(2, 8) - 1);
    setFlags8Bits(chip, chip->a + chip->b);
    chip->a += chip->b;
    printf("ADD B: ADD register A to register B and store result in register A(0x%04X)", chip->a);
    return 4;
}
// bcdehlma
int handle0x81(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) chip->c;
    chip->flags.carry = res > (pow(2, 8) - 1);
    setFlags8Bits(chip, chip->a + chip->c);
    chip->a += chip->c;
    printf("ADD C: ADD register A to register C and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x82(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) chip->d;
    chip->flags.carry = res > (pow(2, 8) - 1);
    setFlags8Bits(chip, chip->a + chip->d);
    chip->a += chip->d;
    printf("ADD D: ADD register A to register D and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x83(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) chip->e;
    chip->flags.carry = res > (pow(2, 8) - 1);
    setFlags8Bits(chip, chip->a + chip->e);
    chip->a += chip->e;
    printf("ADD E: ADD register A to register E and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x84(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) chip->h;
    chip->flags.carry = res > (pow(2, 8) - 1);
    setFlags8Bits(chip, chip->a + chip->h);
    chip->a += chip->h;
    printf("ADD H: ADD register A to register H and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x85(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) chip->l;
    chip->flags.carry = res > (pow(2, 8) - 1);
    setFlags8Bits(chip, chip->a + chip->l);
    chip->a += chip->l;
    printf("ADD B: ADD register A to register L and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x86(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) chip->mem[getHL(chip)];
    chip->flags.carry = res > (pow(2, 8) - 1);
    setFlags8Bits(chip, chip->a + chip->mem[getHL(chip)]);
    chip->a += chip->mem[getHL(chip)];
    printf("ADD M: ADD register A to value at address 0x%04X and store result in register A(0x%04X)", getHL(chip), chip->a);
    return 7;
}

int handle0x87(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) chip->a;
    chip->flags.carry = res > (pow(2, 8) - 1);
    setFlags8Bits(chip, chip->a + chip->a);
    chip->a += chip->a;
    printf("ADD A: ADD register A to register A and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x88(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) chip->b + (u_int16_t) chip->flags.carry;
    setFlags8Bits(chip, chip->a + chip->b + chip->flags.carry);
    chip->a += chip->b + chip->flags.carry;
    chip->flags.carry = res > (pow(2, 8) - 1);
    printf("ADC B: ADD register A to register B and the carry flag and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x89(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) chip->c + (u_int16_t) chip->flags.carry;
    u_int8_t sum = chip->a + chip->c + chip->flags.carry;
    setFlags8Bits(chip, sum);
    chip->a = sum;
    chip->flags.carry = res > (pow(2, 8) - 1);
    printf("ADC C: ADD register A to register C and the carry flag and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x8a(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) chip->d + (u_int16_t) chip->flags.carry;
    u_int8_t sum = chip->a + chip->d + chip->flags.carry;
    setFlags8Bits(chip, sum);
    chip->a = sum;
    chip->flags.carry = res > (pow(2, 8) - 1);
    printf("ADC D: ADD register A to register D and the carry flag and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x8b(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) chip->e + (u_int16_t) chip->flags.carry;
    u_int8_t sum = chip->a + chip->e + chip->flags.carry;
    setFlags8Bits(chip, sum);
    chip->a = sum;
    chip->flags.carry = res > (pow(2, 8) - 1);
    printf("ADC E: ADD register A to register E and the carry flag and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x8c(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) chip->h + (u_int16_t) chip->flags.carry;
    u_int8_t sum = chip->a + chip->h + chip->flags.carry;
    setFlags8Bits(chip, sum);
    chip->a = sum;
    chip->flags.carry = res > (pow(2, 8) - 1);
    printf("ADC H: ADD register A to register H and the carry flag and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x8d(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) chip->l + (u_int16_t) chip->flags.carry;
    u_int8_t sum = chip->a + chip->l + chip->flags.carry;
    setFlags8Bits(chip, sum);
    chip->a = sum;
    chip->flags.carry = res > (pow(2, 8) - 1);
    printf("ADC L: ADD register A to register L and the carry flag and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x8e(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) chip->mem[getHL(chip)] + (u_int16_t) chip->flags.carry;
    u_int8_t sum = chip->a + chip->mem[getHL(chip)] + chip->flags.carry;
    setFlags8Bits(chip, sum);
    chip->a = sum;
    chip->flags.carry = res > (pow(2, 8) - 1);
    printf("ADC M: ADD register A to value at memory address 0x%04X and the carry flag and store result in register A(0x%04X)", getHL(chip), chip->a);
    return 7;
}

int handle0x8f(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) chip->a + (u_int16_t) chip->flags.carry;
    u_int8_t sum = chip->a + chip->a + chip->flags.carry;
    setFlags8Bits(chip, sum);
    chip->a = sum;
    chip->flags.carry = res > (pow(2, 8) - 1);
    printf("ADC A: ADD register A to register A and the carry flag and store result in register A(0x%04X)", chip->a);
    return 4;
}


int handle0x90(Chip* chip, u_int8_t* op) {
    u_int8_t sub = chip->a - chip->b;
    setFlags8Bits(chip, sub);
    chip->a = sub;
    chip->flags.carry = 0;
    printf("SUB B: SUBTRACT register B from register A and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x91(Chip* chip, u_int8_t* op) {
    u_int8_t sub = chip->a - chip->c;
    setFlags8Bits(chip, sub);
    chip->a = sub;
    chip->flags.carry = 0;
    printf("SUB C: SUBTRACT register C from register A and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x92(Chip* chip, u_int8_t* op) {
    u_int8_t sub = chip->a - chip->d;
    setFlags8Bits(chip, sub);
    chip->a = sub;
    chip->flags.carry = 0;
    printf("SUB D: SUBTRACT register D from register A and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x93(Chip* chip, u_int8_t* op) {
    u_int8_t sub = chip->a - chip->e;
    setFlags8Bits(chip, sub);
    chip->a = sub;
    chip->flags.carry = 0;
    printf("SUB E: SUBTRACT register E from register A and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x94(Chip* chip, u_int8_t* op) {
    u_int8_t sub = chip->a - chip->h;
    setFlags8Bits(chip, sub);
    chip->a = sub;
    chip->flags.carry = 0;
    printf("SUB H: SUBTRACT register H from register A and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x95(Chip* chip, u_int8_t* op) {
    u_int8_t sub = chip->a - chip->l;
    setFlags8Bits(chip, sub);
    chip->a = sub;
    chip->flags.carry = 0;
    printf("SUB L: SUBTRACT register D from register A and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x96(Chip* chip, u_int8_t* op) {
    u_int8_t sub = chip->a - chip->mem[getHL(chip)];
    setFlags8Bits(chip, sub);
    chip->a = sub;
    chip->flags.carry = 0;
    printf("SUB M: SUBTRACT value at address 0x%04X from register A and store result in register A(0x%04X)", getHL(chip), chip->a);
    return 7;
}

int handle0x97(Chip* chip, u_int8_t* op) {
    u_int8_t sub = chip->a - chip->a;
    setFlags8Bits(chip, sub);
    chip->a = sub;
    chip->flags.carry = 0;
    printf("SUB A: SUBTRACT register A from register A and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x98(Chip* chip, u_int8_t* op) {
    u_int8_t sub = chip->a - chip->b - chip->flags.carry;
    setFlags8Bits(chip, sub);
    chip->a = sub;
    chip->flags.carry = 0;
    printf("SBB B: SUBTRACT register B and carry flag from register A and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x99(Chip* chip, u_int8_t* op) {
    u_int8_t sub = chip->a - chip->c - chip->flags.carry;
    setFlags8Bits(chip, sub);
    chip->a = sub;
    chip->flags.carry = 0;
    printf("SBB C: SUBTRACT register C and carry flag from register A and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x9a(Chip* chip, u_int8_t* op) {
    u_int8_t sub = chip->a - chip->d - chip->flags.carry;
    setFlags8Bits(chip, sub);
    chip->a = sub;
    chip->flags.carry = 0;
    printf("SBB D: SUBTRACT register D and carry flag from register A and store result in register A(0x%04X)", chip->a);
    return 4;
}


int handle0x9b(Chip* chip, u_int8_t* op) {
    u_int8_t sub = chip->a - chip->e - chip->flags.carry;
    setFlags8Bits(chip, sub);
    chip->a = sub;
    chip->flags.carry = 0;
    printf("SBB E: SUBTRACT register E and carry flag from register A and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x9c(Chip* chip, u_int8_t* op) {
    u_int8_t sub = chip->a - chip->h - chip->flags.carry;
    setFlags8Bits(chip, sub);
    chip->a = sub;
    chip->flags.carry = 0;
    printf("SBB H: SUBTRACT register H and carry flag from register A and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x9d(Chip* chip, u_int8_t* op) {
    u_int8_t sub = chip->a - chip->l - chip->flags.carry;
    setFlags8Bits(chip, sub);
    chip->a = sub;
    chip->flags.carry = 0;
    printf("SBB L: SUBTRACT register L and carry flag from register A and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0x9e(Chip* chip, u_int8_t* op) {
    u_int8_t sub = chip->a - chip->mem[getHL(chip)] - chip->flags.carry;
    setFlags8Bits(chip, sub);
    chip->a = sub;
    chip->flags.carry = 0;
    printf("SBB M: SUBTRACT value at memory address 0x%04X and carry flag from register A and store result in register A(0x%04X)", getHL(chip), chip->a);
    return 7;
}

int handle0x9f(Chip* chip, u_int8_t* op) {
    u_int8_t sub = chip->a - chip->a - chip->flags.carry;
    setFlags8Bits(chip, sub);
    chip->a = sub;
    chip->flags.carry = 0;
    printf("SBB A: SUBTRACT register A and carry flag from register A and store result in register A(0x%04X)", chip->a);
    return 4;
}

int handle0xa0(Chip* chip, u_int8_t* op) {
    chip->a &= chip->b;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("ANA B: USE AND OPERATION ON Register A and B, Store result(0x%04X) in register A", chip->a);
    return 4;
}

int handle0xa1(Chip* chip, u_int8_t* op) {
    chip->a &= chip->c;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("ANA C: USE AND OPERATION ON Register A and C, Store result(0x%04X) in register A", chip->a);
    return 4;

}

int handle0xa2(Chip* chip, u_int8_t* op) {
    chip->a &= chip->d;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("ANA D: USE AND OPERATION ON Register A and D, Store result(0x%04X) in register A", chip->a);
    return 4;
}

int handle0xa3(Chip* chip, u_int8_t* op) {
    chip->a &= chip->e;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("ANA E: USE AND OPERATION ON Register A and E, Store result(0x%04X) in register A", chip->a);
    return 4;
}

int handle0xa4(Chip* chip, u_int8_t* op) {
    chip->a &= chip->h;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("ANA H: USE AND OPERATION ON Register A and H, Store result(0x%04X) in register A", chip->a);
    return 4;
}

int handle0xa5(Chip* chip, u_int8_t* op) {
    chip->a &= chip->l;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("ANA L: USE AND OPERATION ON Register A and L, Store result(0x%04X) in register A", chip->a);
    return 4;
}

int handle0xa6(Chip* chip, u_int8_t* op) {
    chip->a &= chip->mem[getHL(chip)];
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("ANA B: USE AND OPERATION ON Register A and value at memory address 0x%04X, Store result(0x%04X) in register A", getHL(chip), chip->a);
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

int handle0xa8(Chip* chip, u_int8_t* op) {
    chip->a ^= chip->b;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("XRA B: USE EXCLUSIVE OR OPERATION ON Register A and B, Store result(0x%04X) in register A", chip->a);
    return 4;

}

int handle0xa9(Chip* chip, u_int8_t* op) {
    chip->a ^= chip->c;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("XRA C: USE EXCLUSIVE OR OPERATION ON Register A and C, Store result(0x%04X) in register A", chip->a);
    return 4;
}

int handle0xaa(Chip* chip, u_int8_t* op) {
    chip->a ^= chip->d;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("XRA D: USE EXCLUSIVE OR OPERATION ON Register A and D, Store result(0x%04X) in register A", chip->a);
    return 4;

}

int handle0xab(Chip* chip, u_int8_t* op) {
    chip->a ^= chip->e;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("XRA E: USE EXCLUSIVE OR OPERATION ON Register A and E, Store result(0x%04X) in register A", chip->a);
    return 4;
}

int handle0xac(Chip* chip, u_int8_t* op) {
    chip->a ^= chip->h;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("XRA H: USE EXCLUSIVE OR OPERATION ON Register A and H, Store result(0x%04X) in register A", chip->a);
    return 4;

}

int handle0xad(Chip* chip, u_int8_t* op) {
    chip->a ^= chip->l;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("XRA L: USE EXCLUSIVE OR OPERATION ON Register A and L, Store result(0x%04X) in register A", chip->a);
    return 4;

}

int handle0xae(Chip* chip, u_int8_t* op) {
    chip->a ^= chip->mem[getHL(chip)];
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("XRA M: USE EXCLUSIVE OR OPERATION ON Register A and value at memory address, Store result(0x%04X) in register A", getHL(chip), chip->a);
    return 7;

}

// XRA A
int handle0xaf(Chip* chip, u_int8_t* op) {
    chip->a ^= chip->a;
    printf("XRA A: SETS REGISTER A To  A = A ^ A (0x%04X)", chip->a);
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    return 4;
}

int handle0xb0(Chip* chip, u_int8_t* op) {
    chip->a |= chip->b;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("ORA B: USE OR OPERATION ON Register A and B, Store result(0x%04X) in register A", chip->a);
    return 4;
}

int handle0xb1(Chip* chip, u_int8_t* op) {
    chip->a |= chip->c;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("ORA C: USE OR OPERATION ON Register A and C, Store result(0x%04X) in register A", chip->a);
    return 4;

}

int handle0xb2(Chip* chip, u_int8_t* op) {
    chip->a |= chip->d;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("ORA D: USE OR OPERATION ON Register A and D, Store result(0x%04X) in register A", chip->a);
    return 4;
}

int handle0xb3(Chip* chip, u_int8_t* op) {
    chip->a |= chip->e;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("ORA E: USE OR OPERATION ON Register A and E, Store result(0x%04X) in register A", chip->a);
    return 4;
}

int handle0xb4(Chip* chip, u_int8_t* op) {
    chip->a |= chip->h;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("ORA H: USE OR OPERATION ON Register A and H, Store result(0x%04X) in register A", chip->a);
    return 4;
}

int handle0xb5(Chip* chip, u_int8_t* op) {
    chip->a |= chip->l;
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("ORA L: USE OR OPERATION ON Register A and L, Store result(0x%04X) in register A", chip->a);
    return 4;
}

int handle0xb6(Chip* chip, u_int8_t* op) {
    chip->a |= chip->mem[getHL(chip)];
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    printf("ORA M: USE OR OPERATION ON Register A and value at memory addres 0x%04X, Store result(0x%04X) in register A", getHL(chip), chip->a);
    return 7;
}

int handle0xb7(Chip* chip, u_int8_t* op) {
    chip->a |= chip->a;
    setFlags8Bits(chip, chip->a);
    printf("ORA A: USE OR OPERATION ON Register A and A, Store result(0x%04X) in register A", chip->a);
    chip->flags.carry = 0;
    return 4;
}
int handle0xb8(Chip* chip, u_int8_t* op) {
    u_int8_t res = chip->a - chip->b;
    setFlags8Bits(chip, res);
    chip->flags.carry = 0;
    printf("CMP B: SUBTRACT REGISTER B(0x%04X) FROM A(0x%04X) AND SET FLAGS BASED ON RESULT", chip->b, chip->a);
}

int handle0xb9(Chip* chip, u_int8_t* op) {
    u_int8_t res = chip->a - chip->c;
    setFlags8Bits(chip, res);
    chip->flags.carry = 0;
    printf("CMP C: SUBTRACT REGISTER C FROM A AND SET FLAGS BASED ON RESULT");
}

int handle0xba(Chip* chip, u_int8_t* op) {
    u_int8_t res = chip->a - chip->d;
    setFlags8Bits(chip, res);
    chip->flags.carry = 0;
    printf("CMP D: SUBTRACT REGISTER D(0x%04X) FROM A(0x%04X) AND SET FLAGS BASED ON RESULT", chip->d, chip->a);
}

int handle0xbb(Chip* chip, u_int8_t* op) {
    u_int8_t res = chip->a - chip->e;
    setFlags8Bits(chip, res);
    chip->flags.carry = 0;
    printf("CMP E: SUBTRACT REGISTER E FROM A AND SET FLAGS BASED ON RESULT");
}

int handle0xbc(Chip* chip, u_int8_t* op) {
    u_int8_t res = chip->a - chip->h;
    setFlags8Bits(chip, res);
    chip->flags.carry = 0;
    printf("CMP H: SUBTRACT REGISTER H(0x%04X) FROM A(0x%04X) AND SET FLAGS BASED ON RESULT", chip->h, chip->a);
}

int handle0xbd(Chip* chip, u_int8_t* op) {
    u_int8_t res = chip->a - chip->l;
    setFlags8Bits(chip, res);
    chip->flags.carry = 0;
    printf("CMP L: SUBTRACT REGISTER L FROM A AND SET FLAGS BASED ON RESULT");
}

int handle0xbe(Chip* chip, u_int8_t* op) {
    u_int8_t res = chip->a - chip->mem[getHL(chip)];
    setFlags8Bits(chip, res);
    chip->flags.carry = 0;
    printf("CMP M: SUBTRACT value at memory address 0x%04X FROM A AND SET FLAGS BASED ON RESULT", getHL(chip));
}

int handle0xbf(Chip* chip, u_int8_t* op) {
    u_int8_t res = chip->a - chip->a;
    setFlags8Bits(chip, res);
    chip->flags.carry = 0;
    printf("CMP A: SUBTRACT REGISTER A FROM A AND SET FLAGS BASED ON RESULT");
}


int handle0xc0(Chip* chip, u_int8_t* op) {
    printf("CNZ ");
    if (chip->flags.zero == 0) {
        chip->pc = chip->mem[chip->sp] | (chip->mem[chip->sp + 1] << 8);
        chip->sp += 2;
        printf("ZERO FLAG NOT SET, PC Popped from stack: 0x%04X", chip->pc);
        chip->pc -= 1;
        return 2;
    }
    printf("ZERO FLAG SET, NOTHING HAPPENED!");
    return 2;
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

int handle0xc4(Chip* chip, u_int8_t* op) {
    if (chip->flags.zero == 0) {
        u_int16_t newPc = chip->pc + 2;
        chip->mem[chip->sp - 1] = newPc >> 8;
        chip->mem[chip->sp - 2] = newPc & 0xff;
        chip->sp -= 2;
        chip->pc = (op[2] << 8) | op[1];
        printf("CNZ a16: ZERO FLAG NOT SET, PC PUSHED ON STACK, NEW PC: 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        chip->pc += 2;
        printf("CNZ a16: ZErO FLAG SET, NOTHING HAPPENS");
    }
    return 1;
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

int handle0xc7(Chip* chip, u_int8_t* op) {
    u_int16_t saveOnStack = chip->pc + 2;
    chip->mem[chip->sp - 1] = saveOnStack >> 8;
    chip->mem[chip->sp - 2] = saveOnStack & 0xff;
    chip->sp -= 2;
    chip->pc = 0x000;
    printf("RST 0: SAVED PC ON STACK, NEW PC: 0x0000");
    return 11;
}

int handle0xc8(Chip* chip, u_int8_t* op) {
    if (chip->flags.zero == 1) {
        chip->pc = chip->mem[chip->sp] | (chip->mem[chip->sp + 1] << 8);
        chip->sp += 2;
        printf("CZ: ZERO FLAG SET, SO POP PC OFF OF STACK. NEW PC: 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        printf("CZ: ZERO FLAG NOT SET, NO JUMP");
    }
    return 2;
    
}
// RET
int handle0xc9(Chip* chip, u_int8_t* op) {
    chip->pc = (chip->mem[chip->sp + 1] << 8) |chip->mem[chip->sp];
    printf("RET: RETURN TO ADDRESS 0x%04X", chip->pc);
    chip->sp += 2;
    return 10;
}

int handle0xca(Chip* chip, u_int8_t* op) {
    if (chip->flags.zero == 1) {
        chip->pc = (op[2] << 8) | op[1];
        printf("JZ a16: Zero flag set, jump to 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        printf("JZ a16: zero flag not set, no jump");
        chip->pc += 2;
    }
}

int handle0xcb(Chip* chip, u_int8_t* op) {
}

int handle0xcc(Chip* chip, u_int8_t* op) {
    if (chip->flags.zero == 1) {
        u_int16_t newPc = chip->pc + 2;
        chip->mem[chip->sp - 1] = newPc >> 8;
        chip->mem[chip->sp - 2] = newPc & 0xff;
        chip->sp -= 2;
        chip->pc = (op[2] << 8) | op[1];
        printf("CZ a16: ZERO FLAG NOT SET, PC PUSHED ON STACK, NEW PC: 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        chip->pc += 2;
        printf("CZ a16: ZERO FLAG SET, NOTHING HAPPENS");
    }
    return 1;
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

int handle0xce(Chip* chip, u_int8_t* op) {
    u_int16_t res = (u_int16_t) chip->a + (u_int16_t) op[1] + (u_int16_t) chip->flags.carry;
    chip->a += op[1] + chip->flags.carry;
    chip->flags.carry = res > (pow(2, 8) - 1);
    setFlags8Bits(chip, chip->a);
    chip->pc += 1;
    printf("ACI d8: Add immediate and carry flag to register a and store result(0x%04X) in register A", chip->a);
    return 7;
}

// RST 1
int handle0xcf(Chip* chip, u_int8_t* op) {
    chip->mem[chip->sp - 1] = (chip->pc + 2) >> 8;
    chip->mem[chip->sp - 2] = (chip->pc + 2) & 0x00ff;
    printf("RST 1: SAVE PC ON STACK. NEW PC IS 0x0008");
    chip->sp -= 2;
    chip->pc = 8;
    chip->pc -= 1;
    return 11;
}

int handle0xd0(Chip* chip, u_int8_t* op) {
    if (chip->flags.carry == 0) {
        chip->pc = chip->mem[chip->sp] | (chip->mem[chip->sp+1]<<8);
		chip->sp += 2;
        printf("RNC: CARRY FLAG IS NOT SET, SO POP NEW PC FROM STACK: 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        printf("RNC: CARRY FLAG IS SET, SO NOTHING HAPPENS");
    }
    return 2;
}

// POP D
int handle0xd1(Chip* chip, u_int8_t* op) {
    chip->e = chip->mem[chip->sp];
    chip->d = chip->mem[chip->sp + 1];
    chip->sp += 2;
    printf("POP D: POP FROM STACK. REGISTER E SET TO 0x%04X. REGISTER D SET TO 0x%04X. STACK POINTER -= 2", chip->e, chip->d);
    return 10;
}

int handle0xd2(Chip* chip, u_int8_t* op) {
    if (chip->flags.carry == 0) {
        chip->pc = (op[2] << 8) | op[1];
        printf("JNC a16: CARRY FLAG NOT SET, SO WE JUMP TO 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        chip->pc += 2;
        printf("JNC a16: CARRY FLAG SET, SO NOTHING HAPPENS");
    }
    return 10;

}

// OUT
int handle0xd3(Chip* chip, u_int8_t* op) {
    deviceOut(chip, op[1], chip->a);

    printf("OUT: Write value in REGISTER A: 0x%04X to Output Port %d", chip->a, op[1]);
    chip->pc += 1;
    
    return 10;
}

int handle0xd4(Chip* chip, u_int8_t* op) {
    if (chip->flags.carry == 0) {
        
        u_int16_t saveOnStack = chip->pc + 2;
        chip->mem[chip->sp - 1] = saveOnStack >> 8;
        chip->mem[chip->sp - 2] = saveOnStack & 0xff;
        chip->sp -= 2;
        chip->pc = (op[2] << 8) | op[1];
        printf("CNC a16: CARRY FLAG NOT SET SO PC SAVED ON STACK. NEW PC 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        chip->pc += 2;
        printf("CNC a16: CARRY FLAG SET SO NOTHING HAPPENS");
    }
    return 1;
    
}

// PUSH D
int handle0xd5(Chip* chip, u_int8_t* op) {
    chip->mem[chip->sp - 1] = chip->d;
    chip->mem[chip->sp - 2] = chip->e;
    printf("PUSH D: SAVE VALUE AT REGISTER D(0x%04X) and VALUE AT REGISTER E(0x%04X) onto STACK", chip->d, chip->e);
    chip->sp -= 2;
    return 11;
}

int handle0xd6(Chip* chip, u_int8_t* op) {
    u_int8_t res = chip->a - op[1];
    chip->flags.carry = chip->a < op[1];
    setFlags8Bits(chip, res);
    chip->a = res;
    chip->pc += 1;
    printf("SUI d8: SUBTRACT IMMEDIATE FROM REGISTER A. STORE RESULT IN REGISTER A: 0x%04X", chip->a);
    return 7;
}

// RST 2
int handle0xd7(Chip* chip, u_int8_t* op) {
    chip->mem[chip->sp - 1] = chip->pc >> 8;
    chip->mem[chip->sp - 2] = chip->pc & 0x00ff;
    chip->sp -= 2;
    chip->pc = 16;
    chip->pc -= 1;
    return 11;
}

int handle0xd8(Chip* chip, u_int8_t* op) {
    if (chip->flags.carry == 1) {
        chip->pc = chip->mem[chip->sp] | (chip->mem[chip->sp + 1] << 8);
        chip->sp += 2;
         printf("RC: CARRY FLAG IS SET: POP NEW PC FROM STACK 0x%04X", chip->pc);
         chip->pc -= 1;
    } else {
        printf("CARRY FLAG NOT SET, SO NOTHING HAPPENS");
    }
    return 1;
}

int handle0xd9(Chip* chip, u_int8_t* op) {
    
}

int handle0xda(Chip* chip, u_int8_t* op) {
    if (chip->flags.carry == 1) {
        chip->pc = (op[2] << 8) | op[1];
        printf("JC a16: CARRY flag set, jump to 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        printf("JC a16: carry flag not set, no jump");
        chip->pc += 2;
    }
    return 10;
}

// IN
int handle0xdb(Chip* chip, u_int8_t* op) {
    deviceIn(chip, op[1]);
    printf("IN: Data from Port %d written to Register A: 0x%04X", op[1], chip->a);
    chip->pc += 1;
    return 10;
}

int handle0xdc(Chip* chip, u_int8_t* op) {
    if (chip->flags.carry == 1) {
        u_int16_t newPc = chip->pc + 2;
        chip->mem[chip->sp - 1] = newPc >> 8;
        chip->mem[chip->sp - 2] = newPc & 0xff;
        chip->sp -= 2;
        chip->pc = (op[2] << 8) | op[1];
        printf("CC a16: CARRY FLAG NOT SET, PC PUSHED ON STACK, NEW PC: 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        chip->pc += 2;
        printf("CC a16: CARRY FLAG SET, NOTHING HAPPENS");
    }
    return 1;
}

int handle0xdd(Chip* chip, u_int8_t* op) {
    
}

int handle0xde(Chip* chip, u_int8_t* op) {
    u_int16_t res = chip->a - op[1] - chip->flags.carry;
    chip->flags.carry = res > (pow(2, 8) - 1);
    setFlags8Bits(chip, res);
    chip->pc += 1;
    chip->a = res;
    printf("SBI d8: SUBTRACT IMMEDIATE AND CARRY FlAG FROM REGISTER A, SET FLAGS, STORE RESULT IN REGISTER A (0x%04X)", chip->a);
}

int handle0xdf(Chip* chip, u_int8_t* op) {
    u_int16_t saveOnStack = chip->pc + 2;
    chip->mem[chip->sp - 1] = saveOnStack >> 8;
    chip->mem[chip->sp - 2] = saveOnStack & 0xff;
    chip->sp -= 2;
    chip->pc = 0x0018;
    printf("RST 3: SAVED PC ON STACK, NEW PC: 0x0018");
    chip->pc -= 1;
    return 11;
}

int handle0xe0(Chip* chip, u_int8_t* op) {
    if (chip->flags.parity == 0) {
        chip->pc = chip->mem[chip->sp] | (chip->mem[chip->sp+1]<<8);
		chip->sp += 2;
        printf("RPO: PARITY FLAG NOT SET, SO WE JUMP TO 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        printf("RPO: PARITY FLAG SET, SO NOTHING HAPPENS");
    }
    return 2;
}
// POP H
int handle0xe1(Chip* chip, u_int8_t* op) {
    chip->l = chip->mem[chip->sp];
    chip->h = chip->mem[chip->sp + 1];
    chip->sp += 2;
    printf("POP H: POP FROM STACK. REGISTER L SET TO 0x%04X. REGISTER H SET TO 0x%04X. STACK POINTER -= 2", chip->l, chip->h);
    return 10;
}

int handle0xe2(Chip* chip, u_int8_t* op) {
    if (chip->flags.parity == 0) {
        chip->pc = (op[2] << 8) | op[1];
        printf("JPO a16: SINCE THE PARITY FLAG IS NOT SET, we jump to address: 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        printf("JPO a16: PARITY FLAG IS SET, NO JUMP");
        chip->pc += 2;
    }
    return 10;
}

int handle0xe3(Chip* chip, u_int8_t* op) {
    u_int8_t h = chip->h;
    u_int8_t l = chip->l;
    chip->l = chip->mem[chip->sp];
    chip->h = chip->mem[chip->sp + 1];
    chip->mem[chip->sp] = l;
    chip->mem[chip->sp + 1] = h;
    printf("XTHL: HL REGISTER AND TOP OF STACK SWAP VALUES. NEW HL REGISTER: 0x%04X", getHL(chip));
}

int handle0xe4(Chip* chip, u_int8_t* op) {
    if (chip->flags.parity == 0) {
        u_int16_t saveOnStack = chip->pc + 2;
        chip->mem[chip->sp - 1] = saveOnStack >> 8;
        chip->mem[chip->sp - 2] = saveOnStack & 0xff;
        chip->sp -= 2;
        chip->pc = (op[2] << 8) | op[1];
        printf("CPO a16: PARITY FLAG NOT SET SO PC SAVED ON STACK. NEW PC 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        chip->pc += 2;
        printf("CPO a16: PARITY FLAG SET SO NOTHING HAPPENS");
    }
    return 1;
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
    printf("pARITY FLAG: 0x%04X\n", chip->flags.parity);
    chip->pc += 1;
    return 7;
}

int handle0xe7(Chip* chip, u_int8_t* op) {
    u_int16_t saveOnStack = chip->pc + 2;
    chip->mem[chip->sp - 1] = saveOnStack >> 8;
    chip->mem[chip->sp - 2] = saveOnStack & 0xff;
    chip->sp -= 2;
    chip->pc = 0x020;
    printf("RST 4: SAVED PC ON STACK, NEW PC: 0x0020");
    chip->pc -= 1;
    return 11;
}

int handle0xe8(Chip* chip, u_int8_t* op) {
    if (chip->flags.parity == 1) {
        chip->pc = chip->mem[chip->sp] | (chip->mem[chip->sp + 1] << 8);
        chip->sp += 2;
         printf("RPE: PARITY FLAG IS SET: POP NEW PC FROM STACK 0x%04X", chip->pc);
         chip->pc -= 1;
    } else {
        printf("RPE: PARITY FLAG NOT SET, SO NOTHING HAPPENS");
    }
    return 1;
}

int handle0xe9(Chip* chip, u_int8_t* op) {
    chip->pc = getHL(chip);
    printf("PCHL: SET PC TO HL REGISTER HL 0x%04X", chip->pc);
    chip->pc -= 1;
    return 5;
}

int handle0xea(Chip* chip, u_int8_t* op) {
    if (chip->flags.parity == 1) {
        chip->pc = (op[2] << 8) | op[1];
        printf("JPE a16: PARITY flag set, jump to 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        printf("JPE a16: parity flag not set, no jump");
        chip->pc += 2;
    }
    return 10;
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

int handle0xec(Chip* chip, u_int8_t* op) {
    if (chip->flags.parity == 1) {
        u_int16_t newPc = chip->pc + 2;
        chip->mem[chip->sp - 1] = newPc >> 8;
        chip->mem[chip->sp - 2] = newPc & 0xff;
        chip->sp -= 2;
        chip->pc = (op[2] << 8) | op[1];
        printf("CPE a16: PARITY FLAG NOT SET, PC PUSHED ON STACK, NEW PC: 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        chip->pc += 2;
        printf("CPE a16: PARITY FLAG SET, NOTHING HAPPENS");
    }
    return 1;
}

int handle0xed(Chip* chip, u_int8_t* op) {
    
}

int handle0xee(Chip* chip, u_int8_t* op) {
    chip->a ^= op[1];
    chip->flags.carry = 0;
    setFlags8Bits(chip, chip->a);
    printf("XRI d8: A = A ^ immediate = (0x%04X)", chip->a);
    chip->pc += 1;
    return 7;
}

int handle0xef(Chip* chip, u_int8_t* op) {
    u_int16_t saveOnStack = chip->pc + 2;
    chip->mem[chip->sp - 1] = saveOnStack >> 8;
    chip->mem[chip->sp - 2] = saveOnStack & 0xff;
    chip->sp -= 2;
    chip->pc = 0x028;
    printf("RST 5: SAVED PC ON STACK, NEW PC: 0x0028");
    chip->pc -= 1;
    return 11;
}

int handle0xf0(Chip* chip, u_int8_t* op) {
    if (chip->flags.sign == 0) {
        chip->pc = chip->mem[chip->sp] | (chip->mem[chip->sp+1]<<8);
		chip->sp += 2;
        printf("RP: SIGN FLAG IS NOT SET, SO POP NEW PC FROM STACK: 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        printf("RP: SIGN FLAG IS SET, SO NOTHING HAPPENS");
    }
    return 2;
}
// POP PSW
int handle0xf1(Chip* chip, u_int8_t* op) {
    u_int8_t psw = chip->mem[chip->sp];
    chip->a = chip->mem[chip->sp + 1];
    chip->flags.carry = (0x05 == (psw & 0x08));
    chip->flags.parity = (0x04 == (psw & 0x04));
    chip->flags.sign = (0x02 == (psw & 0x02));
    chip->flags.zero = (0x01 == (psw & 0x01));
    // *(unsigned char*) &chip->flags = chip->mem[chip->sp];
    printf("POP PSW");
    chip->sp += 2;
    return 10;
}

int handle0xf2(Chip* chip, u_int8_t* op) {
    if (chip->flags.sign == 0) {
        chip->pc = (op[2] << 8) | op[1];
        printf("JP a16: SINCE THE SIGN FLAG IS NOT SET, we jump to address: 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        printf("JP a16: SIGN FLAG IS SET, NO JUMP");
        chip->pc += 2;
    }
    return 10;
}

int handle0xf3(Chip* chip, u_int8_t* op) {
    chip->interrupt_enabled = 0;
    printf("DI: Interrupts toggled off");
}

int handle0xf4(Chip* chip, u_int8_t* op) {
    if (chip->flags.sign == 0) {
        u_int16_t saveOnStack = chip->pc + 2;
        chip->mem[chip->sp - 1] = saveOnStack >> 8;
        chip->mem[chip->sp - 2] = saveOnStack & 0xff;
        chip->sp -= 2;
        chip->pc = (op[2] << 8) | op[1];
        printf("CP a16: SIGN FLAG NOT SET SO PC SAVED ON STACK. NEW PC 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        chip->pc += 2;
        printf("CP a16: SIGN FLAG SET SO NOTHING HAPPENS");
    }
    return 1;
}
// PUSH PSW
int handle0xf5(Chip* chip, u_int8_t* op) {
    chip->mem[chip->sp - 1] = chip->a;
    u_int8_t psw = 2;
    // psw = 00000010
    psw |= chip->flags.carry; //psw = 00000011
    psw |= (chip->flags.parity << 2);//psw = 00000111
    psw |= (chip->flags.auxCarry << 4);//psw = 00001111
    psw |= (chip->flags.zero << 6); //psw = 00000111
    psw |= (chip->flags.sign << 7);
    printf("PUSH PSW");
    chip->mem[chip->sp - 2] = *(unsigned char*)&chip->flags;
    chip->sp -= 2;


    return 11;
}

int handle0xf6(Chip* chip, u_int8_t* op) {
    chip->a |= op[1];
    setFlags8Bits(chip, chip->a);
    chip->flags.carry = 0;
    chip->pc += 1;
    printf("ORI d8: A = A | immediate = (0x%04X)", chip->a);
}

int handle0xf7(Chip* chip, u_int8_t* op) {
    u_int16_t saveOnStack = chip->pc + 2;
    chip->mem[chip->sp - 1] = saveOnStack >> 8;
    chip->mem[chip->sp - 2] = saveOnStack & 0xff;
    chip->sp -= 2;
    chip->pc = 0x030;
    printf("RST 6: SAVED PC ON STACK, NEW PC: 0x0030");
    chip->pc -= 1;
    return 11;
}

int handle0xf8(Chip* chip, u_int8_t* op) {
    if (chip->flags.sign == 1) {
        chip->pc = chip->mem[chip->sp] | (chip->mem[chip->sp + 1] << 8);
        chip->sp += 2;
         printf("RM: SIGN FLAG IS SET: POP NEW PC FROM STACK 0x%04X", chip->pc);
         chip->pc -= 1;
    } else {
        printf("RM: SIGN FLAG NOT SET, SO NOTHING HAPPENS");
    }
    return 1;
}

int handle0xf9(Chip* chip, u_int8_t* op) {
    chip->sp = getHL(chip);
    printf("SPHL: SET STACK POINTER TO HL: 0x%04X", chip->sp);
}

int handle0xfa(Chip* chip, u_int8_t* op) {
    if (chip->flags.sign == 1) {
        chip->pc = (op[2] << 8) | op[1];
        printf("JM a16: sign flag set, jump to 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        printf("JM a16: sign flag not set, no jump");
        chip->pc += 2;
    }
    return 10;
}
// EI
int handle0xfb(Chip* chip, u_int8_t* op) {
    chip->interrupt_enabled = 1;
    printf("EI: INTERRUPT ENABLE FlAG SET TO 1");
    return 4;
}

int handle0xfc(Chip* chip, u_int8_t* op) {
    if (chip->flags.sign == 1) {
        u_int16_t newPc = chip->pc + 2;
        chip->mem[chip->sp - 1] = newPc >> 8;
        chip->mem[chip->sp - 2] = newPc & 0xff;
        chip->sp -= 2;
        chip->pc = (op[2] << 8) | op[1];
        printf("CM a16: SIGN FLAG NOT SET, PC PUSHED ON STACK, NEW PC: 0x%04X", chip->pc);
        chip->pc -= 1;
    } else {
        chip->pc += 2;
        printf("CM a16: SIGNFLAG SET, NOTHING HAPPENS");
    }
    return 1;
}

int handle0xfd(Chip* chip, u_int8_t* op) {

}
// CPI d8
int handle0xfe(Chip* chip, u_int8_t* op) {
    u_int8_t res = chip->a - op[1];
    chip->flags.carry = chip->a < op[1];
    setFlags8Bits(chip, res);
    chip->pc += 1;
    printf("SUBTRACT IMMEDIATE FROM REGISTER A(0x%04X) AND USE RESULT TO SET FLAGS", res);
    return 7;
}

int handle0xff(Chip* chip, u_int8_t* op) {
    u_int16_t saveOnStack = chip->pc + 2;
    chip->mem[chip->sp - 1] = saveOnStack >> 8;
    chip->mem[chip->sp - 2] = saveOnStack & 0xff;
    chip->sp -= 2;
    chip->pc = 0x038;
    printf("RST 7: SAVED PC ON STACK, NEW PC: 0x0038");
    chip->pc -= 1;
    return 11;
}