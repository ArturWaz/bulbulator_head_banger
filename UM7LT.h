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
//#include "GLOBAL_DATA_FROM_SOURCES.h"




class UM7LT : private base_UM7LT, private PortCOM {

public:

    enum Data : uint16_t {
        NONE         = 0x0000,
        ACC_PROC     = 0x0001,
        GRYO_PROC    = 0x0002,
        MAG_PROC     = 0x0004,
        EULER        = 0x0008,
        QUAT         = 0x0010
    };

    UM7LT(int const portNumber): PortCOM(portNumber,115200) { open(); }
//    UM7LT(char const *host, char const *port): ClientTCP(host,port) {}
    ~UM7LT() {}

    uint16_t getData() {
        uint16_t data = NONE;

        uint8_t buff[254];
        uint8_t read = readBlock(buff, 250);
        if (!read) return NONE;

        Packet packets[15];
        uint8_t parsed = parseData(buff, read, packets, 15);
        if (!parsed) return NONE;

        for (int i = 0; i < parsed; ++i) {

            uint8_t pos = 0;
            switch (packets[i].address) {

                case 0x55:
                    ;

                case 0x61: // gryo processed

                    data |= Data::GRYO_PROC;
                    pos += 16;
                    if (pos == packets[i].dataLength) break;

                case 0x65: // accel processed

                    data |= Data::ACC_PROC;
                    pos += 16;
                    if (pos == packets[i].dataLength) break;

                case 0x69: // mag processed

                    data |= Data::MAG_PROC;
                    pos += 16;
                    if (pos == packets[i].dataLength) break;

                case 0x6D: // quternion

                    data |= Data::QUAT;
                    pos += 12;
                    if (pos == packets[i].dataLength) break;

                case 0x70: // euler

                    data |= Data::EULER;
                    pos += 20;
                    if (pos == packets[i].dataLength) break;

                default: // default
                    std::cerr << "\tUnknown packet, function UM7LT::getData()\n";
                    break;
            }

        }

        return data;
    }

};


#endif //__UM7LT_H_