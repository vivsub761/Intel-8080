
#include "main.h"

ioDevices* InitializeDevices() {
    ioDevices* devices = malloc(sizeof(ioDevices));
    return devices;
}

u_int8_t deviceIn(Chip* chip, u_int8_t port) {
    switch(port) {
        case 1:
            chip->a =  chip->devices->read1;
            break;
        case 2:
            chip->a = chip->devices->read2;
            break;
        case 3:
            chip->a =  (chip->devices->shiftRegister >> (8 - chip->devices->shiftRegResOffset)) & 0xff;
            break;
        default:
            break;
    }
}

void deviceOut(Chip* chip, u_int8_t port, u_int8_t data) {
    switch(port) {
        case 2: chip->devices->shiftRegResOffset = data;
        case 4: chip->devices->shiftRegister = (data << 8) | (chip->devices->shiftRegister >> 8);

    }

}