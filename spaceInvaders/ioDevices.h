#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef IODEVICES_H
#define IODEVICES_H
typedef struct {
    u_int8_t read1;
    u_int8_t read2;
    u_int8_t read3_sr;
    u_int8_t shiftRegResOffset;
    u_int16_t shiftRegister;
} ioDevices;

#endif