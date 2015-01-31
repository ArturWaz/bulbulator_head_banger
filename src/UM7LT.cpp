/*
 * UM7LT.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */

#include "UM7LT.h"



UM7_LT::UM7_LT(int portNumber): portCOM(portNumber,115200) {
	// TODO Auto-generated constructor stub

}

UM7_LT::~UM7_LT() {
	// TODO Auto-generated destructor stub
}


void UM7_LT::threadedReading() {
//    readDataThread = std::thread(this->readData);
//    readDataThread.detach();
}

void UM7_LT::readData() {

    int packetPose = 0;
    uint8_t packet[256];

    while (true){
        try {
            uint8_t ch[128] = {0};
            portCOM.readBlock(ch,128);
            for (int i = 0; ch[i] != 0; ++i) {

                // COMBINE PACKET
                if (ch[i] == '$'){
                    packetPose = 0;
                }
                else if (packetPose > 5 && packet[packetPose-2] == '*'){
                    // todo send packet to other thread?
                }
                packet[packetPose++] = ch[i];

            }
        } catch(bool e){}
    }
}