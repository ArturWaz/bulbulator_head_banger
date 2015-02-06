/*
 * MiniMaestro.h
 *
 *  Created on: 31.01.15
 *      Author: artur
 */


#ifndef MINIMAESTRO_H_
#define MINIMAESTRO_H_

#include <cstdint>
#include "PortCOM.h"


// todo Make test of this class


class MiniMaestro: private PortCOM {

public:

    MiniMaestro(int port);
    ~MiniMaestro();

    void setPosition(uint8_t channel, uint16_t position);

    void setPosition(uint8_t channel, uint8_t position);

    void setSpeed(uint8_t channel, uint16_t speed);

    void setAcceleration(uint8_t channel, uint16_t acceleration);

};



#endif // MINIMAESTRO_H_