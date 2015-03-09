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
#include "GLOBAL_DATA_FROM_SOURCES.h"




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

        union UIF { uint32_t in; float out; };
        union UII { uint16_t in; int16_t out; };
        UIF toFloat[5];
        UII toInt[5];

        for (int i = 0; i < parsed; ++i) {

            uint8_t pos = 0;
            switch (packets[i].address) {

                case 0x55:
                    ;

                case 0x61: // gryo processed
                    // todo
                    data |= Data::GRYO_PROC;
                    pos += 16;
                    if (pos == packets[i].dataLength) break;

                case 0x65: // accel processed
                    toFloat[0].in = (packets[i].data[pos+0]<<24) | (packets[i].data[pos+1]<<16) | (packets[i].data[pos+2]<<8) | (packets[i].data[pos+3]<<0);
                    toFloat[1].in = (packets[i].data[pos+4]<<24) | (packets[i].data[pos+5]<<16) | (packets[i].data[pos+6]<<8) | (packets[i].data[pos+7]<<0);
                    toFloat[2].in = (packets[i].data[pos+8]<<24) | (packets[i].data[pos+9]<<16) | (packets[i].data[pos+10]<<8) | (packets[i].data[pos+11]<<0);
                    toFloat[3].in = (packets[i].data[pos+12]<<24) | (packets[i].data[pos+13]<<16) | (packets[i].data[pos+14]<<8) | (packets[i].data[pos+15]<<0);
                    GlobalData::AccelProcessed::buffer.push(Vector3DTime(double(toFloat[0].out), double(toFloat[1].out), double(toFloat[2].out), double(toFloat[3].out)));
                    data |= Data::ACC_PROC;
                    pos += 16;
                    if (pos == packets[i].dataLength) break;

                case 0x69: // mag processed
                    // todo
                    data |= Data::MAG_PROC;
                    pos += 16;
                    if (pos == packets[i].dataLength) break;

                case 0x6D: // quternion
                    // todo
                    data |= Data::QUAT;
                    pos += 12;
                    if (pos == packets[i].dataLength) break;

                case 0x70: // euler
                    toInt[0].in = (packets[i].data[pos+0]<<8) | (packets[i].data[pos+1]<<0);
                    toInt[1].in = (packets[i].data[pos+2]<<8) | (packets[i].data[pos+3]<<0);
                    toInt[2].in = (packets[i].data[pos+4]<<8) | (packets[i].data[pos+5]<<0);
                    toFloat[3].in = (packets[i].data[pos+16]<<24) | (packets[i].data[pos+17]<<16) | (packets[i].data[pos+18]<<8) | (packets[i].data[pos+19]<<0);
                    GlobalData::EulerAngle::buffer.push(Vector3DTime(double(toInt[0].out)/91.02222, double(toInt[1].out)/91.02222, double(toInt[2].out)/91.02222, double(toInt[3].out)));
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