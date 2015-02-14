/*
 * UM7_LT.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#include <DefineFunctions.h>
#include "UM7_LT.h"


#define UM7_LT_BUFFER uint8_t(250)



UM7_LT::UM7_LT(int portNumer): PortCOM(portNumer,115200), readData(false) {
    PortCOM::open();
}

UM7_LT::UM7_LT(int portNumber, int baudrate): PortCOM(portNumber,baudrate), readData(false) {
    PortCOM::open();
}

UM7_LT::~UM7_LT() {}



//#define UM7_LT_DATA_READER_FUNCION \


void dataReader(UM7_LT *um7_lt) {
    UM7_LT_packet packet;
    uint8_t data[UM7_LT_BUFFER];
    uint8_t packetIndex = 0;
    uint8_t tempPacket[80] = {0};
    int dataLength = 0;
    int read;
    while (um7_lt->readData){
        read = um7_lt->PortCOM::readBlock(data, UM7_LT_BUFFER);
        for (int i = 0; i < read; ++i) {

            if (dataLength == 0) {
                if (tempPacket[3] != 0) {
                    if (!(tempPacket[3] & 0x80)) dataLength = 7;
                    else if (!(tempPacket[3] & 0x40)) dataLength = 11;
                    else dataLength = (tempPacket[3] & 0x3C) + 7;
                }
            }
            else if (dataLength == packetIndex){
                uint16_t checksum = 0;
                for (int i = 0; i < dataLength-2; ++i) checksum += uint16_t(tempPacket[i]);
                if (checksum != ((tempPacket[dataLength-2]<<8) | tempPacket[dataLength-1])){
                    ERROR_COM(um7_lt->getPortNumber(), "Checksum is incorrect.");
                }
                else if (dataLength > 7) {
                    packet.address = tempPacket[4];
                    packet.length = uint8_t(dataLength - 7);
                    packet.data = new uint8_t[packet.length];
                    for (int i = 0; i < packet.length; ++i) packet.data[i] = tempPacket[i+5];
                    um7_lt->readThreadMutex.lock();
                    um7_lt->packets.push(packet);
                    um7_lt->readThreadMutex.unlock();
                }
                tempPacket[3] = 0;
                packetIndex = 0;
                dataLength = 0;
            }
            else if (dataLength < 7 || dataLength >= 80){
                tempPacket[3] = 0;
                packetIndex = 0;
                dataLength = 0;
            }


            tempPacket[packetIndex++] = data[i];
        }
    }
}

void UM7_LT::turnOnThreadedRead() {
    readData = true;
    readThread = std::thread(dataReader,this);
}

void UM7_LT::turnOffThreadedRead() {
    readData = false;
    readThread.join();
}

void UM7_LT::sendPacket(UM7_LT_packet const &aConst) const {

}

bool UM7_LT::takeLastPacket(UM7_LT_packet &packet) {
    readThreadMutex.lock();
    if (packets.empty()){
        readThreadMutex.unlock();
        return false;
    }
    packet = packets.front();
    packets.pop();
    readThreadMutex.unlock();
    return true;
}
