/*
 * _base_UM7LT.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#ifndef _base_UM7LT_H_
#define _base_UM7LT_H_



class _base_UM7LT {

    static uint8_t const maxLength = 99;
    uint8_t const *endPtr;
    uint8_t data[maxLength+1] = {};

//    uint8_t tmpTable[3] = {0x00,0x00,0x00};

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
        uint8_t dataLength;
        uint8_t data[maxLength];
    };

    _base_UM7LT(): actualPtr(&(data[0])), endPtr(&(data[maxLength])) {}

    inline uint8_t length() { return maxLength; }

    inline uint8_t parseData(uint8_t const *buffer, uint8_t const length, Packet *packets, uint8_t const packetsLength) {
        uint8_t numberOfPackets = 0;
        for (uint8_t *i = const_cast<uint8_t*>(&(buffer[0])); i != &(buffer[length]); ++i) {

            if (*i == 's') {
                data[0] = *i;
            }
            else if (data[0] == 's' && *i == 'n') {
                data[1] = *i;
            }
            else if (data[0] == 's' && data[1] == 'n' && *i == 'p') {

                if (numberOfPackets < packetsLength ) {
                    uint8_t dataLength = (0x3C & (data[3]));
                    uint16_t checksum = 's'+'n'+'p';

                    for (uint8_t *i = &(data[3]); i != &(data[5+dataLength]); ++i) {
                        checksum += *i;
                    }

//                    {
//                        std::cout << "dataLength: " << int(dataLength) << std::endl;
//                        std::cout << std::hex;
//                        for (int i = 0; i < packetsLength; ++i) {
//                            std::cout << int(data[i]) << " ";
//                        }
//                        std::cout << std::dec << std::endl;
//                    }
//                    std::cout << "calculated checksum: " << int(checksum) << " to compare: " << ((data[5+dataLength]<<8) | (data[6+dataLength])) << std::endl;

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

                actualPtr = &(data[2]);
//                data[0] = 's'; data[1] = 'n';
                data[0] = 0; data[1] = 0;
            }
            else {
                data[0] = 0; data[1] = 0;
            }

            if (actualPtr == endPtr) {
                actualPtr = &(data[0]);
                data[0] = 0; data[1] = 0;
                throw Exception("Too many badly sent packets, function: _base_UM7LT::parseData(uint8_t*,uint8_t)");
            }

            *actualPtr = *i;
            ++actualPtr;
        }
        return numberOfPackets;
    }

};


#endif //__UM7_LT_H_