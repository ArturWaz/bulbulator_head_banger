
#include "MiniMaestro.h"


MiniMaestro::MiniMaestro(int port): PortCOM(port,115200){
    PortCOM::open();
}

MiniMaestro::~MiniMaestro() {}

void MiniMaestro::setPosition(uint8_t channel, uint16_t position) {
    uint8_t buf[] = { 0x84, channel, position & 0x7F, (position >> 7) & 0x7F };
    PortCOM::sendBlock(buf, 4);
}

void MiniMaestro::setPosition(uint8_t channel, uint8_t position) {
    uint8_t buf[] = { 0xFF, channel, position };
    PortCOM::sendBlock(buf, 3);
}

void MiniMaestro::setSpeed(uint8_t channel, uint16_t speed) {
    uint8_t buf[] = { 0x87, channel, speed & 0x7F, (speed >> 7) & 0x7F };
    PortCOM::sendBlock(buf, 4);
}

void MiniMaestro::setAcceleration(uint8_t channel, uint16_t acceleration) {
    uint8_t buf[] = { 0x87, channel, acceleration & 0x7F, (acceleration >> 7) & 0x7F };
    PortCOM::sendBlock(buf, 4);
}
