/*
 * UM7LT.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */

#include <iostream>
#include <queue>
#include <mutex>
#include <iomanip>
#include "UM7LT.h"
#include "DefineFunctions.h"
#include "CONFIG.h"


UM7_LT::UM7_LT(int portNumber): PortCOM(portNumber,115200), timeFrame(1.0) {
	PortCOM::open();
}

UM7_LT::~UM7_LT() {}





std::queue<Packet> packets;
std::mutex PacketQueueMutex;

void readData(UM7_LT *um7_lt) {

    int packetPose;
    Packet packet;
    packet.packet[0] = '\n';

    while (true) {
        try {
            uint8_t ch[128] = {0};
            int read = um7_lt->PortCOM::readBlock(ch, 128);
            for (int i = 0; i < read; ++i) {
                if ((ch[i] == '$' && ch[i+1] == 'P' && ch[i+2] == 'C') || (ch[i] == 's' && ch[i+1] == 'n' && ch[i+2] == 'p')){
                    packet.packetLength = packetPose;
                    PacketQueueMutex.lock();
                    packets.push(packet);
                    PacketQueueMutex.unlock();
//                    packet.coutPacket("Read data: ");
                    packetPose = 0;
                }
                packet.packet[packetPose++] = ch[i];
            }
        } catch (bool e) {}
    }
}


void parseDataPackets(UM7_LT *um7_lt) {
    Packet packet;
    while (true) {

        PacketQueueMutex.lock();
        bool empty = packets.empty();
        PacketQueueMutex.unlock();

        if (empty) continue;

        PacketQueueMutex.lock();
        packet = packets.front();
        packets.pop();
        PacketQueueMutex.unlock();

//        packet.coutPacket("Data parser: ");

        if (packet.compareToString("$PCHR", 5)) {
            um7_lt->parseNMEApacket(packet);
        }
        else if (packet.compareToString("snp", 3)){
            um7_lt->parseBinaryPacket(packet);
        }
        else {
            if (packet.packet[0] != '\n') ERROR_COM(um7_lt->getPortNumber(), "Could not recognize the packet type.");
        }
    }
}







void Packet::coutPacket(const char*string)const{
    std::cout << string;
    for (int i = 0; i < packetLength /*packet[i] != '\n' && packet[i] != 0 && i < packet.size()*/; ++i) {
        std::cout << (char) packet[i];
    }
    std::cout << std::endl;
}

bool Packet::compareToString(const char *string, int numberOfCharacters)const{
    for (int i = 0; i < numberOfCharacters; ++i)
        if (packet[i] != string[i]) return false;
    return true;
}


//void Packet::convertToChar(char *pointer)const{
//    for (int i = 0; i < 256 && packet[1] != 0; ++i) {
//        pointer[i] = packet[i];
//    }
//}


void UM7_LT::threadedReading() {
    readDataThread = std::thread(readData,this);
    readDataThread.detach();
    parseDataThread = std::thread(parseDataPackets,this);
    parseDataThread.detach();
}



#define GET_DOUBLES_FROM_PACKET \
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
        ERROR_COM(PortCOM::getPortNumber(),"Incorrect checksum."); \
        return; \
    } \
    char*pEnd;



void UM7_LT::parseNMEApacket(const Packet &packet) { // todo convert degrees to radians
    ERROR_COM(PortCOM::getPortNumber(), "Try to avoid using NMEA packets :)");
    char temp[256] = {};
    uint8_t calculatedChecksum = 'P';
    if (packet.compareToString("$PCHRH",6)){ // Health packet
//        packet.coutPacket("Health packet: ");
    }
    if (packet.compareToString("$PCHRA",6)){ // Euler Angles packet
        EulerAnglesTime eulerAnglesTime;

        GET_DOUBLES_FROM_PACKET

        eulerAnglesTime.setTime(strtod(temp,&pEnd));
        eulerAnglesTime.setPhi(strtod(pEnd,&pEnd));
        eulerAnglesTime.setTheta(strtod(pEnd,&pEnd));
        eulerAnglesTime.setPsi(strtod(pEnd,NULL));

        std::cout<<"NMEA euler angles:    "<<std::fixed<<std::setprecision(6)<<eulerAnglesTime.getTime()<<",  \t"<<eulerAnglesTime.getPhi()<<",  \t"<<eulerAnglesTime.getTheta()<<",  \t"<<eulerAnglesTime.getPsi()<<std::endl;

        bool clear = false;
        if (eulerList.size()) {
            clear = true;
            if ((eulerAnglesTime.getTime() - eulerList.front().getTime()) < timeFrame)
                clear = false;
        }

        eulerListMutex.lock();
        eulerList.push_back(eulerAnglesTime);
        if (clear) eulerList.pop_front();
        eulerListMutex.unlock();
        return;
    }
    if (packet.compareToString("$PCHRS,1",8)) { // choose accelerometer
        Accelerometer accelerometer;
        const double gravityConstant = 9.8; // recommended, page: 4, from: http://www.chrobotics.com/docs/AN-1008-SensorsForOrientationEstimation.pdf


        GET_DOUBLES_FROM_PACKET

        strtod(temp, &pEnd);
        accelerometer.setTime(strtod(pEnd, &pEnd));
        accelerometer.setAx(strtod(pEnd, &pEnd)*gravityConstant);
        accelerometer.setAy(strtod(pEnd, &pEnd)*gravityConstant);
        accelerometer.setAz(strtod(pEnd, NULL)*gravityConstant);

        bool clear = false;
        if (accelerometerList.size()) {
            clear = true;
            if ((accelerometer.getTime() - accelerometerList.front().getTime()) < timeFrame)
                clear = false;
        }

        std::cout << "NMEA accelerometer:   " << std::fixed << std::setprecision(4) << accelerometer.getTime() << ',' << accelerometer.getAx() << "," << accelerometer.getAy() << "," << accelerometer.getAz() << std::endl;
//
//        if (!eulerList.empty()) {
//            accelerometer.eleminitateGravity(eulerList.front());
//            std::cout << "Without gravi: " << std::fixed << std::setprecision(4) << accelerometer.getTime() << ',' << accelerometer.getAx() << "," << accelerometer.getAy() << "," << accelerometer.getAz() << ", " << std::hex << (int) calculatedChecksum << std::dec << std::endl;
//        }

        accelerometerListMutex.lock();
        accelerometerList.push_back(accelerometer);
        if (clear) accelerometerList.pop_front();
        accelerometerListMutex.unlock();
        return;
    }
    if (packet.compareToString("$PCHRQ,",7)) { // quaternions
        QuaternionTime quaternion;


        GET_DOUBLES_FROM_PACKET


        quaternion.setTime(strtod(temp, &pEnd));
        quaternion.setA(strtod(pEnd, &pEnd));
        quaternion.setB(strtod(pEnd, &pEnd));
        quaternion.setC(strtod(pEnd, &pEnd));
        quaternion.setD(strtod(pEnd, NULL));

        bool clear = false;
        if (quaternionList.size()) {
            clear = true;
            if ((quaternion.getTime() - quaternionList.front().getTime()) < timeFrame)
                clear = false;
        }


        std::cout << "NMEA quaternion:      " << std::fixed << std::setprecision(6) << quaternion.getTime() << ",  \t" << quaternion.getA() << ",  \t" << quaternion.getB() << ",  \t" << quaternion.getC() << ",  \t" << quaternion.getD() << std::endl;


        quaternionListMutex.lock();
        quaternionList.push_back(quaternion);
        if (clear) quaternionList.pop_front();
        quaternionListMutex.unlock();
        return;
    }
}



void UM7_LT::parseBinaryPacket(const Packet &packet) {

    /*    PACKET TYPE BLOCK    */
    int dataLength;
    if (!(packet[3]&0x80)) dataLength = 0;   // check if the packet has data
    else if (!(packet[3]&0x40)) dataLength = 4;
    else dataLength = packet[3]&0x3C;
    if (dataLength+7 != packet.packetLength){
        ERROR_COM(PortCOM::getPortNumber(), "Calculated data length and read packet length are different.");
        return;
    }

    if (packet[3]&0x02) std::cout<<"Hidden address.\n"; // not sure if necessary in reading packet
    if (packet[3]&0x01) std::cout<<"Command failed.\n"; // not sure if necessary in reading packet


    /*    COMPARE CHECKSUMS    */
    uint16_t checksum = 0;
    for (int i = 0; i < packet.packetLength-2; ++i) checksum += uint16_t(packet[i]);
    if (checksum != ((packet[packet.packetLength-2]<<8) | packet[packet.packetLength-1])){
        ERROR_COM(PortCOM::getPortNumber(), "Checksum is incorrect.");
        return;
    }

//    std::cout<<std::hex<<int(packet[4])<<std::endl; // to recognize the address of the package

    /*    ADDRESS IDENTIFICATION BLOCK    */
    union FloatIntUnion { uint32_t ui; float f; };
    union uIntIntUnion { uint16_t ui; int16_t i; };
    FloatIntUnion convertIF;
    uIntIntUnion convertuII;

    if (packet[4] == 0x70){ // packet contains euler angles and angles rates with time
        const double pi = 3.1415926535897932384626433832795028841971693993751058;
        EulerAnglesTime eulerAnglesTime;

        convertuII.ui = (packet[5]<<8) | packet[6];
        eulerAnglesTime.setPhi((double(convertuII.i)*pi) / (91.02222*360));  // (double(convertuII.i) / 91.02222);//
        convertuII.ui =(packet[7]<<8) | packet[8];
        eulerAnglesTime.setTheta((double(convertuII.i)*pi) / (91.02222*360));
        convertuII.ui = (packet[9]<<8) | packet[10];
        eulerAnglesTime.setPsi((double(convertuII.i)*pi) / (91.02222*360));
        convertIF.ui = (packet[21]<<24) | (packet[22]<<16) | (packet[23]<<8) | packet[24];
        eulerAnglesTime.setTime(double(convertIF.f));

//        std::cout<<"Binary euler angles:  "<<std::fixed<<std::setprecision(6)<<eulerAnglesTime.getTime()<<",  \t"<<eulerAnglesTime.getPhi()<<",  \t"<<eulerAnglesTime.getTheta()<<",  \t"<<eulerAnglesTime.getPsi()<<std::endl;

        bool clear = false;
        if (eulerList.size()) {
            clear = true;
            if ((eulerAnglesTime.getTime() - eulerList.front().getTime()) < timeFrame)
                clear = false;
        }

        eulerListMutex.lock();
        eulerList.push_back(eulerAnglesTime);
        if (clear) eulerList.pop_front();
        eulerListMutex.unlock();
        return;
    }
    if (packet[4] == 0x6D){ // read quaternion data
        QuaternionTime quaternion;

        convertuII.ui = (packet[5]<<8) | packet[6];
        quaternion.setA(double(convertuII.i) / 29789.09091);
        convertuII.ui = (packet[7]<<8) | packet[8];
        quaternion.setB(double(convertuII.i) / 29789.09091);
        convertuII.ui = (packet[9]<<8) | packet[10];
        quaternion.setC(double(convertuII.i) / 29789.09091);
        convertuII.ui = (packet[11]<<8) | packet[12];
        quaternion.setD(double(convertuII.i) / 29789.09091);
        convertIF.ui = (packet[13]<<24) | (packet[14]<<16) | (packet[15]<<8) | packet[16];
        quaternion.setTime(double(convertIF.f));

        bool clear = false;
        if (quaternionList.size()) {
            clear = true;
            if ((quaternion.getTime() - quaternionList.front().getTime()) < timeFrame)
                clear = false;
        }


//        std::cout << "Binary quaternion:    " << std::fixed << std::setprecision(6) << quaternion.getTime() << ",  \t" << quaternion.getA() << ",  \t" << quaternion.getB() << ",  \t" << quaternion.getC() << ",  \t" << quaternion.getD() << std::endl;
//        /*if (quaternion.norm() < 0.999985)*/ std::cout<<quaternion.norm()<<std::endl;

        quaternionListMutex.lock();
        quaternionList.push_back(quaternion);
        if (clear) quaternionList.pop_front();
        quaternionListMutex.unlock();
        return;
    }
    if (packet[4] == 0x65){ // read processed accelerometer data
        const double gravityConstant = 9.8; // recommended: 9.8, page: 4, from: http://www.chrobotics.com/docs/AN-1008-SensorsForOrientationEstimation.pdf
        Accelerometer accelerometer;

        convertIF.ui = (packet[5]<<24) | (packet[6]<<16) | (packet[7]<<8) | packet[8];
        accelerometer.setAx(double(convertIF.f)*gravityConstant);
        convertIF.ui = (packet[9]<<24) | (packet[10]<<16) | (packet[11]<<8) | packet[12];
        accelerometer.setAy(double(convertIF.f)*gravityConstant);
        convertIF.ui = (packet[13]<<24) | (packet[14]<<16) | (packet[15]<<8) | packet[16];
        accelerometer.setAz(double(convertIF.f)*gravityConstant);
        convertIF.ui = (packet[17]<<24) | (packet[18]<<16) | (packet[19]<<8) | packet[20];
        accelerometer.setTime(double(convertIF.f));

        //std::cout << "Binary accelerometer: " << std::fixed << std::setprecision(4) << accelerometer.getTime() << ",\t" << accelerometer.getAx() << ",  \t" << accelerometer.getAy() << ",  \t" << accelerometer.getAz()  << std::endl;
        std::cout << "Accelerometer norm: " << sqrt(accelerometer.getAx()*accelerometer.getAx() + accelerometer.getAy()*accelerometer.getAy() + accelerometer.getAz()*accelerometer.getAz()) << std::endl;

//        if (!eulerList.empty()) {
//            accelerometer.eleminitateGravity(eulerList.front());
//            std::cout << "Without gravi:        " << std::fixed << std::setprecision(4) << accelerometer.getTime() << ",\t" << accelerometer.getAx() << ",  \t" << accelerometer.getAy() << ",  \t" << accelerometer.getAz() << std::endl;
//        }

        bool clear = false;
        if (accelerometerList.size()) {
            clear = true;
            if ((accelerometer.getTime() - accelerometerList.front().getTime()) < timeFrame)
                clear = false;
        }

        accelerometerListMutex.lock();
        accelerometerList.push_back(accelerometer);
        if (clear) accelerometerList.pop_front();
        accelerometerListMutex.unlock();
        return;
    }
}






void Accelerometer::eleminitateGravity(EulerAnglesTime aConst){ // todo doesnt work

//    aConst.setPsi((aConst.getPsi()*PI)/360.0);
//    aConst.setTheta((aConst.getTheta()*PI)/360.0);
//    aConst.setPhi((aConst.getPhi()*PI)/360.0);

    RotationMatrix rotationMatrix;
    RotationMatrix R_I_v1;
    RotationMatrix R_v1_v2;
    RotationMatrix R_v2_B;

    R_I_v1.xRotation(-1*aConst.getPsi());
    R_v1_v2.yRotation(-1*aConst.getTheta());
    R_v2_B.zRotation(-1*aConst.getPhi());

    rotationMatrix = R_I_v1*R_v1_v2;
    rotationMatrix = rotationMatrix*R_v2_B;

    double x, y, z;
    x = ax*rotationMatrix[0] + ay*rotationMatrix[1] + az*rotationMatrix[2];
    y = ax*rotationMatrix[3] + ay*rotationMatrix[4] + az*rotationMatrix[5];
    z = ax*rotationMatrix[6] + ay*rotationMatrix[7] + az*rotationMatrix[8];
    ax = x;
    ay = y;
    az = z + GRAVITY_CONSTANT;
}

void Accelerometer::eleminitateGravity(QuaternionTime quaternionTime) { // todo doesnt work
    RotationMatrix rotationMatrix;
    quaternionTime.toRotationMatrix(rotationMatrix);

    double x, y, z;
    x = ax*rotationMatrix[0] + ay*rotationMatrix[1] + az*rotationMatrix[2];
    y = ax*rotationMatrix[3] + ay*rotationMatrix[4] + az*rotationMatrix[5];
    z = ax*rotationMatrix[6] + ay*rotationMatrix[7] + az*rotationMatrix[8];
    ax = x;
    ay = y;
    az = z + GRAVITY_CONSTANT;
}
