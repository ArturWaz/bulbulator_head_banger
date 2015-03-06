/*
 * UM7LT.h
 *
 *  Created on: 3/5/2015
 *      Author: Artur
 */


#ifndef UM7LT_H_
#define UM7LT_H_


#include "serial/ClientTCP.h"
#include "serial/PortCOM.h"
#include "base_UM7LT.h"





class UM7LT : private base_UM7LT, private PortCOM {

public:

    enum data {
        ACC_PROC     = 0x0001,
        GRYO_PROC    = 0x0002,
        MAG_PROC     = 0x0004,
        EULER        = 0x0008,
        QUAT         = 0x0010
    };

    UM7LT(int const portNumber): PortCOM(portNumber,115200) {}
//    UM7LT(char const *host, char const *port): ClientTCP(host,port) {}
    ~UM7LT() {}

    uint16_t getData()

};


#endif //__UM7LT_H_