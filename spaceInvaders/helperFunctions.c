#include "main.h"

u_int16_t getBC(Chip* chip) {
    return (chip->b << 8) | chip->c;
}

u_int16_t getDE(Chip* chip) {
    return (chip->d << 8) | chip->e;
}

u_int16_t getHL(Chip* chip) {
    return (chip->h << 8) | chip->l;
}
void setDandEtoDE(Chip* chip, u_int16_t de) {
    chip->d = de >> 8;
    chip->e = de & 0x00ff;
}

void setHandLtoHL(Chip* chip, u_int16_t hl) {
    chip->h = hl >> 8;
    chip->l = hl & 0x00ff;
}

void setBandCtoBC(Chip* chip, u_int16_t bc) {
    chip->b = bc >> 8;
    chip->c = bc & 0x00ff;
}
bool evenOneBits(int num) {
    bool parity = true;
    while (num > 0) {
        if (num & 1) {
            parity = !parity;
        }
        num = num >> 1;
    }
    return parity;
}
void setFlags8Bits(Chip* chip, u_int8_t res) {
    chip->flags->sign = (res >> 7);
    chip->flags->zero = (res == 0);
    chip->flags->parity = evenOneBits(res);
}

void setFlags16Bits(Chip* chip, u_int16_t res) {
    chip->flags->sign = (res >> 15);
    chip->flags->zero = (res == 0);
    chip->flags->parity = evenOneBits(res);
}
