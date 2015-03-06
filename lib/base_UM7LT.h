/*
 * _base_UM7LT.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#ifndef base_UM7LT_H_
#define base_UM7LT_H_


#include <iostream>


class base_UM7LT {

    static uint8_t const maxLength = 99;
    uint8_t const *endPtr;
    uint8_t data[maxLength+1] = {};

    uint8_t *actualPtr;
    uint8_t *endDataPtr;
    uint8_t dataLength;

public:

    class Exception : public std::exception {
        char const *info;
    public:
        explicit Exception(char const *info): info(info) {}
        virtual char const *what() const throw() { return info; }
    };

    struct Packet {
        uint8_t packetType;
        uint8_t address;
        uint8_t dataLength;
        uint8_t data[maxLength];
    };

    base_UM7LT(): actualPtr(&(data[3])), endPtr(&(data[maxLength])), endDataPtr(&(data[0])), dataLength(0) {}

    inline uint8_t length() { return maxLength; }

    inline uint8_t parseData(uint8_t const *buffer, uint8_t const length, Packet *packets, uint8_t const packetsLength) {
        uint8_t numberOfPackets = 0;

        for (uint8_t *i = const_cast<uint8_t*>(&(buffer[0])); i != &(buffer[length]); ++i) {

            uint8_t *prevPtr = actualPtr;
            if (*(--prevPtr) == 'p' && !dataLength)
                if (*(--prevPtr) == 'n')
                    if (*(--prevPtr) == 's') {
                        dataLength = (0x3C & (*i));
                        actualPtr = &(data[3]);
                        endDataPtr = &(data[dataLength+7]);
                    }

            *actualPtr = *i;
            ++actualPtr;

            if (actualPtr == endDataPtr && dataLength > 0) {

                if (numberOfPackets < packetsLength ) {
                    uint16_t checksum = 's'+'n'+'p';

                    for (uint8_t *i = &(data[3]); i != &(data[5+dataLength]); ++i)
                        checksum += *i;

                    if (checksum == ((data[5+dataLength]<<8) | (data[6+dataLength]))) {
                        Packet *packet = &(packets[numberOfPackets]);
                        packet->packetType = data[3];
                        packet->dataLength = dataLength;
                        packet->address = data[4];
                        memcpy(&(packet->data[0]),&(data[5]),packet->dataLength*sizeof(uint8_t));
                        ++numberOfPackets;
                    }
                }
                else {
                    std::cerr << "\tPacket lost, too small [Packet *packets] array.\n";
                }

                dataLength = 0;
                actualPtr = &(data[3]);
            }

            if (actualPtr == endPtr) {
                actualPtr = &(data[3]);
                data[0] = 0; data[1] = 0;
                throw Exception("Too many badly sent packets, function: base_UM7LT::parseData(uint8_t*,uint8_t)");
            }
        }
        return numberOfPackets;
    }

};


#endif //__UM7_LT_H_