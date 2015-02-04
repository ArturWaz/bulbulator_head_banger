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
            for (int i = 0; ch[i] != 0; ++i) {
                // COMBINE PACKET
                if (ch[i] == '$') {
                    packetPose = 0;
                    read = false;
                }
                else if (packetPose > 10 && packet.packet[packetPose - 3] == '*') {
                    if (!read) {
                        packet.packet[packetPose] = '\0';

//                        std::cout << "Packet: ";
//                        for (int i = 0; i < packetPose; ++i) {
//                            std::cout << packet.packet[i];
//                        }
//                        std::cout << std::endl;

                        mtx.lock();
                        packets.push(packet);
                        mtx.unlock();
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

            std::cout << "Packet: ";
            for (int i = 0; packet.packet[i] != '\0'; ++i) {
                std::cout << packet.packet[i];
            }
            std::cout << std::endl;

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


void UM7_LT::parseNMEApacket(const Packet &packet) {
    char temp[256] = {};
    int k = 0;
    char checksum[] = "00";
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
        for (int i = 0; i < packet.packet.size(); ++i) {

            if (i > 1 && packet.packet[i] != '*')
                calculatedChecksum ^= packet.packet[i];

            if (packet.packet[i] == ','){
                temp[k++] = ' ';
                continue;
            }
            if ((packet.packet[i] >= '0' && packet.packet[i] <= '9') || packet.packet[i] == '.' || packet.packet[i] == '-'){
                temp[k++] = packet.packet[i];
                continue;
            }
            if (packet.packet[i] == '*'){
                checksum[0] = packet.packet[i+1];
                checksum[1] = packet.packet[i+2];
                break;
            }
        }
        char*pEnd;
        eulerAnglesTime.setTime(strtod(temp,&pEnd));
        eulerAnglesTime.setPhi(strtod(pEnd,&pEnd));
        eulerAnglesTime.setTheta(strtod(pEnd,&pEnd));
        eulerAnglesTime.setPsi(strtod(pEnd,NULL));

        std::cout<<std::fixed<<std::setprecision(3)<<eulerAnglesTime.getTime()<<','<<eulerAnglesTime.getPhi()<<","<<eulerAnglesTime.getTheta()<<","<<eulerAnglesTime.getPsi();
        // todo check checksum

        std::cout<<", "<<std::hex<<(int)calculatedChecksum<<std::dec<<std::endl;
    }
    if (packet.compareToString("$PCHRS,2",8)){ // Accelerometer
    }
}

