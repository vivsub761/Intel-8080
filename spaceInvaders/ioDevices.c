
#include "main.h"

ioDevices* InitializeDevices() {
    ioDevices* devices = malloc(sizeof(ioDevices));
    return devices;
}

u_int8_t deviceIn(Chip* chip, u_int8_t port) {
    switch(port) {
        case 0x1:
            chip->a =  chip->devices->read1;
            break;
        case 0x2:
            chip->a = chip->devices->read2;
            break;
        case 0x3:
            chip->a =  (chip->devices->shiftRegister >> (8 - chip->devices->shiftRegResOffset)) & 0xff;
            break;
        default:
            break;
    }
}

void deviceOut(Chip* chip, u_int8_t port, u_int8_t data) {
    switch(port) {
        // change register offset
        case 0x2: chip->devices->shiftRegResOffset = data & 0x7;
        // perform register shift
        case 0x4: chip->devices->shiftRegister = (data << 8) | (chip->devices->shiftRegister >> 8);

    }

}