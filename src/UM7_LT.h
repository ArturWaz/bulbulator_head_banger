/*
 * UM7_LT.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#ifndef UM7_LT_H_
#define UM7_LT_H_


#include <com/PortCOM.h>
#include <thread>
#include <mutex>
#include <queue>
#include <cstring>
#include <iostream>


struct UM7_LT_packet {
    uint8_t address;
    uint8_t length;
    uint8_t *data;

    UM7_LT_packet(): data(NULL) {}
    UM7_LT_packet(UM7_LT_packet const &p): address(p.address), length(p.length), data(p.data) {}
    inline void operator=(UM7_LT_packet const &p) {
        address = p.address;
        length = p.length;
        data = p.data;
    }
    ~UM7_LT_packet() { if (data != NULL) free(data); }
};


class UM7_LT: public PortCOM {

    std::thread readThread;
    std::mutex readThreadMutex;
    bool readData;

    std::queue<UM7_LT_packet> packets;

    friend void dataReader(UM7_LT*);

public:

    UM7_LT(int portNumer);
    UM7_LT(int portNumber, int baudrate);
    ~UM7_LT();

    void turnOnThreadedRead();
    void turnOffThreadedRead();

    void sendPacket(const UM7_LT_packet &)const;

    bool takeLastPacket(UM7_LT_packet &);

};


#endif //__UM7_LT_H_