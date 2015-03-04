/*
 * _base_UM7LT.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#ifndef _base_UM7LT_H_
#define _base_UM7LT_H_


#include <serial/PortCOM.h>
#include <thread>
#include <mutex>
#include <queue>
#include <cstring>
#include <iostream>


//struct UM7_LT_packet {
//    uint8_t address;
//    uint8_t length;
//    uint8_t *data;
//
//    UM7_LT_packet(): data(NULL) {}
//    UM7_LT_packet(UM7_LT_packet const &p): address(p.address), length(p.length), data(p.data) {}
//    inline void operator=(UM7_LT_packet const &p) {
//        address = p.address;
//        length = p.length;
//        data = p.data;
//    }
//    ~UM7_LT_packet() { if (data != NULL) free(data); }
//};


class _base_UM7LT : public PortCOM {

    std::thread readThread;
    std::mutex readThreadMutex;
    bool readData;

//    std::queue<UM7_LT_packet> packets;

    friend void dataReader(_base_UM7LT *) {}

public:

    struct Packet {

        uint8_t size;
        uint8_t *packet;

        Packet(): size(0), packet(nullptr) {}
        Packet(uint8_t size): size(size) { packet = (uint8_t*)malloc(size*sizeof(uint8_t)); }
        Packet(uint8_t *array, uint8_t length): size(length) {
            packet = (uint8_t*)malloc(size*sizeof(uint8_t));
            memcpy(packet,array,size);
        }
        Packet(Packet &p): packet(p.packet), size(p.size) { p.packet = nullptr; }
        ~Packet() { if (packet != nullptr) free(packet); }

        void move(Packet &p) {
            if (packet != nullptr) free(packet);
            packet = p.packet;
            size = p.size;
            p.size = 0;
        }

    private:
        void operator=(Packet&) {}
    };

    _base_UM7LT(int portNumber): PortCOM(portNumber,115200), readData(false) { PortCOM::open(); }
    _base_UM7LT(int portNumber, int baudrate): PortCOM(portNumber,baudrate), readData(false) { PortCOM::open(); }
    ~_base_UM7LT() { turnOffThreadedRead(); }

    void turnOnThreadedRead() {
        if (readData) return;
        readData = true;
//        readThread = std::thread(dataReader,this);
    }
    void turnOffThreadedRead() {
        if (!readData) return;
        readData = false;
        readThread.join();
    }

    void sendPacket(Packet const &packet) const {
//        if (!packet.length) return;
//        uint8_t tmpPacket[7+packet.length];
//        uint16_t checksum = 0;
//
//        tmpPacket[0] = 's'; tmpPacket[1] = 'n'; tmpPacket[2] = 'p';
//        tmpPacket[3] = packetType;
//        tmpPacket[4] = packet.address;
//        for (int i = 0; i < packet.length+7; ++i) {
//            if (i > 4) tmpPacket[i] = packet.data[i - 5];
//            checksum += tmpPacket[i];
//        }
//        tmpPacket[packet.length+5] = uint8_t(checksum>>8);
//        tmpPacket[packet.length+6] = uint8_t(checksum);
//
//        PortCOM::sendBlock(tmpPacket,7+packet.length);
    }

    bool getLastPacket(Packet &);

};


#endif //__UM7_LT_H_