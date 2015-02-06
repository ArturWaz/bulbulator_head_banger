/*
 * UM7LT.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */

#include <iostream>
#include <queue>
#include <mutex>
#include <array>
#include <iomanip>
#include "UM7LT.h"
#include "DefineFunctions.h"


UM7_LT::UM7_LT(int portNumber): portCOM(portNumber,115200), timeFrame(1.0) {
	portCOM.open();
}

UM7_LT::~UM7_LT() {}





std::queue<Packet> packets;
std::mutex mtx;


void readData(UM7_LT *um7_lt) {

    int packetPose = 0;
    bool read = true; // check if packet is read
    Packet packet;

    while (true) {
        try {
            uint8_t ch[128] = {0};
            um7_lt->portCOM.readBlock(ch, 128);
            for (int i = 0; ch[i] != 0 && i < 128; ++i) {

                /* COMBINE PACKET */
                if (ch[i] == '$') {
                    packetPose = 0;
                    read = false;
                }
                else if (packetPose > 10 && packet.packet[packetPose - 3] == '*') {
                    if (!read) {
                        packet.packet[packetPose] = '\0';

                        std::cout << "Packet(read): ";
                        for (int i = 0; i < packetPose; ++i) {
                            std::cout << packet.packet[i];
                        }
                        std::cout << std::endl;

//                        mtx.lock();
//                        packets.push(packet);
//                        mtx.unlock();

                        read = true;
                    }
                    break;
                }
                packet.packet[packetPose++] = ch[i];
            }
        } catch (bool e) {}
    }
}


void parseDataPackets(UM7_LT *um7_lt) {
    Packet packet;
    while (true) {

        mtx.lock();
        bool empty = packets.empty();
        mtx.unlock();

        if (empty) continue;

        mtx.lock();
        packet = packets.front();
        packets.pop();
        mtx.unlock();

        if (packet.compareToString("$PCHR", 5)) {

//            std::cout << "Packet: ";
//            for (int i = 0; packet.packet[i] != '\0'; ++i) {
//                std::cout << packet.packet[i];
//            }
//            std::cout << std::endl;

            um7_lt->parseNMEApacket(packet);

        }
        else {
            ERROR_COM(um7_lt->portCOM.getPortNumber(), "Could not recognize the packet type.");
        }
    }
}








bool Packet::compareToString(const char *string, int numberOfCharacters)const{
    for (int i = 0; i < numberOfCharacters; ++i)
        if (packet[i] != string[i]) return false;
    return true;
}


void Packet::convertToChar(char *pointer)const{
    for (int i = 0; i < 256 && packet[1] != '\0'; ++i) {
        pointer[i] = packet[i];
    }
}


void UM7_LT::threadedReading() {
    readDataThread = std::thread(readData,this);
    readDataThread.detach();
    parseDataThread = std::thread(parseDataPackets,this);
    parseDataThread.detach();
}



#define GET_DOUBLE_FROM_PACKET \
    int k = 0; \
    char checksum[] = "00"; \
    for (int i = 0; i < packet.packet.size(); ++i) { \
        if (i > 1 && packet.packet[i] != '*') \
            calculatedChecksum ^= packet.packet[i]; \
        if (packet.packet[i] == ','){ \
            temp[k++] = ' '; \
            continue; \
        } \
        if ((packet.packet[i] >= '0' && packet.packet[i] <= '9') || packet.packet[i] == '.' || packet.packet[i] == '-'){ \
            temp[k++] = packet.packet[i]; \
            continue; \
        } \
        if (packet.packet[i] == '*'){ \
            checksum[0] = packet.packet[i+1]; \
            checksum[1] = packet.packet[i+2]; \
            break; \
        } \
    } \
    if (strtol(checksum,NULL,16) != calculatedChecksum) { \
        ERROR_COM(portCOM.getPortNumber(),"Incorrect checksum."); \
        return; \
    } \
    char*pEnd;



void UM7_LT::parseNMEApacket(const Packet &packet) {
    char temp[256] = {};
    uint8_t calculatedChecksum = 'P';
    if (packet.compareToString("$PCHRH",6)){ // Health packet
//            std::cout << "Health packet: ";
//            for (int i = 0; packet.packet[i] != '\0'; ++i) {
//                std::cout << packet.packet[i];
//            }
//            std::cout << std::endl;
    }
    if (packet.compareToString("$PCHRA",6)){ // Euler Angles packet
        EulerAnglesTime eulerAnglesTime;

        GET_DOUBLE_FROM_PACKET

        eulerAnglesTime.setTime(strtod(temp,&pEnd));
        eulerAnglesTime.setPhi(strtod(pEnd,&pEnd));
        eulerAnglesTime.setTheta(strtod(pEnd,&pEnd));
        eulerAnglesTime.setPsi(strtod(pEnd,NULL));

//        std::cout<<"Euler angles: "<<std::fixed<<std::setprecision(3)<<eulerAnglesTime.getTime()<<','<<eulerAnglesTime.getPhi()<<","<<eulerAnglesTime.getTheta()<<","<<eulerAnglesTime.getPsi()<<", "<<std::hex<<(int)calculatedChecksum<<std::dec<<std::endl;
        bool clear = false;
        if (eulerList.size()) {
            clear = true;
            if ((eulerAnglesTime.getTime() - eulerList.front().getTime()) < timeFrame)
                clear = false;
        }

        mtx.lock();
        eulerList.push_back(eulerAnglesTime);
        if (clear) eulerList.pop_front();
        mtx.unlock();
        return;
    }
    if (packet.compareToString("$PCHRS,1",8)){ // choose accelerometer
        Accelerometer accelerometer;

        GET_DOUBLE_FROM_PACKET

        strtod(temp, &pEnd);
        accelerometer.setTime(strtod(pEnd, &pEnd));
        accelerometer.setAx(strtod(pEnd,&pEnd));
        accelerometer.setAy(strtod(pEnd, &pEnd));
        accelerometer.setAz(strtod(pEnd, NULL));

        bool clear = false;
        if (accelerometerList.size()) {
            clear = true;
            if ((accelerometer.getTime() - accelerometerList.front().getTime()) < timeFrame)
                clear = false;
        }

//        std::cout<<"Accelerometer: "<<std::fixed<<std::setprecision(4)<<accelerometer.getTime()<<','<<accelerometer.getAx()<<","<<accelerometer.getAy()<<","<<accelerometer.getAz()<<", "<<std::hex<<(int)calculatedChecksum<<std::dec<<std::endl;

        mtx.lock();
        accelerometerList.push_back(accelerometer);
        if (clear) accelerometerList.pop_front();
        mtx.unlock();
        return;
    }
}

