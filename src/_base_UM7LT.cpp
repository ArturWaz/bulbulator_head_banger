/*
 * UM7_LT.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#include <DefineFunctions.h>
#include "_base_UM7LT.h"

using namespace std;


#define UM7_LT_BUFFER uint8_t(250)



_base_UM7LT::_base_UM7LT(int portNumer): PortCOM(portNumer,115200), readData(false) {
    PortCOM::open();
}

_base_UM7LT::_base_UM7LT(int portNumber, int baudrate): PortCOM(portNumber,baudrate), readData(false) {
    PortCOM::open();
}

_base_UM7LT::~_base_UM7LT() {}


void showPacket_UM7_LT(UM7_LT_packet const &p){
    cout << "read:   " << hex;
    for (int i = 0; i < p.length; ++i) {
        cout << " " << int(p.data[i]);
    }
    cout << dec << endl;
}


void dataReader(_base_UM7LT *um7_lt) {
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
                if ((checksum != ((tempPacket[dataLength-2]<<8) | tempPacket[dataLength-1])) || tempPacket[0] != 's' || tempPacket[1] != 'n' || tempPacket[2] != 'p'){
                    ERROR_COM(um7_lt->getPortNumber(), "Checksum is incorrect.");
                }
                else if (dataLength > 7) {
                    packet.address = tempPacket[4];
                    packet.length = uint8_t(dataLength - 7);
                    packet.data = (uint8_t*) malloc(packet.length*sizeof(uint8_t));
                    memcpy(packet.data,tempPacket,packet.length*sizeof(uint8_t));
//                    showPacket_UM7_LT(packet);
                    um7_lt->readThreadMutex.lock();
                    um7_lt->packets.push(packet);
                    um7_lt->readThreadMutex.unlock();
                    packet.data = NULL;
                }
                tempPacket[3] = 0;
                packetIndex = 0;
                dataLength = 0;
            }
            else if (dataLength < 7 || packetIndex > 78){
                tempPacket[3] = 0;
                packetIndex = 0;
                dataLength = 0;
                ERROR_COM(um7_lt->getPortNumber(), "Packet/s lost.");
            }

            tempPacket[packetIndex++] = data[i];
        }
    }
}

void _base_UM7LT::turnOnThreadedRead() {
    readData = true;
    readThread = std::thread(dataReader,this);
}

void _base_UM7LT::turnOffThreadedRead() {
    readData = false;
    readThread.join();
}

void _base_UM7LT::sendPacket(UM7_LT_packet const &aConst) const {

}

bool _base_UM7LT::takeLastPacket(UM7_LT_packet &packet) {
    readThreadMutex.lock();
    if (packets.empty()){
        readThreadMutex.unlock();
        return false;
    }
    packet = packets.front();
    packets.front().data = NULL;
    packets.pop();
    readThreadMutex.unlock();
    return true;
}
