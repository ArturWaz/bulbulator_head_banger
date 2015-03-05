/*
 * _base_UM7LT.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#ifndef _base_UM7LT_H_
#define _base_UM7LT_H_




class _base_UM7LT {

    static uint8_t const maxLength = 80;
    uint8_t const *endPtr;
    uint8_t data[maxLength+1] = {};

    uint8_t tmpTable[] = {0,0,0};

    uint8_t *actualPtr;

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
        uint8_t data[maxLength];
    };

    _base_UM7LT(): actualPtr(&(data[0])), endPtr(&(data[maxLength])) {}

    inline uint8_t length() { return maxLength; }

    inline uint8_t parseData(uint8_t const *buffer, uint8_t const length, Packet *packets, uint8_t const maxNumberOfPackets) {
        uint8_t numberOfPackets = 0;
        for (uint8_t *i = 0; i != &(buffer[length]); ++i) {

            if (*i == 's') {
                tmpTable[0] = *i;
            }
            else if (tmpTable[0] == 's' && *i == 'n') {
                tmpTable[1] = *i;
            }
            else if (tmpTable[0] == 's' && tmpTable[1] == 'n' && *i == 'p') {

                //todo parse data

                actualPtr = &(data[3]);
                data[0] = 's'; data[1] = 'n';
                tmpTable[0] = 0; tmpTable[1] = 0;
                ++numberOfPackets;
            }
            else {
                tmpTable[0] = 0; tmpTable[1] = 0;
            }

            if (actualPtr == endPtr) {
                actualPtr = &(data[0]);
                tmpTable[0] = 0; tmpTable[1] = 0;
                throw Exception("Badly parsed packet, function: _base_UM7LT::parseData(uint8_t*,uint8_t)");
            }

            *actualPtr = *i;
            ++actualPtr;
        }
        return numberOfPackets;
    }

};


#endif //__UM7_LT_H_