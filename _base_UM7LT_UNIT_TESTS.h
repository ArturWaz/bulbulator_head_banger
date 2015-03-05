/*
 * _base_UM7LT_UNIT_TESTS.h
 *
 *  Created on: 3/5/2015
 *      Author: Artur
 */


#ifndef _base_UM7LT_UNIT_TESTS_H_
#define _base_UM7LT_UNIT_TESTST_H_

#include <cstdint>
#include <cstdlib>
#include <iostream>

#include "PortCOM.h"

namespace _base_UM7LT_UNIT_TESTS {
    using namespace std;

    void writeArray (uint8_t *a, uint8_t l) {
        if (a == nullptr) { cerr << "array pointer is null\n"; return; }
        cout << hex;
        for (int i = 0; i < l; ++i) {
            cout << int(a[i]) << " ";
        }
        cout << dec << endl;
    }

    void showPacket(_base_UM7LT::Packet const &p) {
        cout << hex << endl;
        cout << "Address: 0x" << int(p.address) << ", packet type: 0x" << int(p.packetType) << endl;
        cout << "Data length: " << dec << int(p.dataLength) << hex << endl;
        for (int i = 0; i < p.dataLength; ++i) {
            cout << int(p.data[i]) << " ";
        }
        cout << dec << endl;
    }

    void parseData() { // generate proper input data and parse it
        uint8_t buffer[250];

        buffer[0] = 's'; buffer[1] = 'n'; buffer[2] = 'p'; buffer[3] = 0x3C;
        int i;
        uint16_t checksum = 's'+'n'+'p'+buffer[3];
        for (i = 4; i < buffer[3]+5; ++i) {
            buffer[i] = rand()%255;
            checksum += buffer[i];
        }
        buffer[i] = (checksum>>8); buffer[++i] = (checksum);
        cout << "input checksum: " << int(checksum) << endl;

        buffer[++i] = 's'; buffer[++i] = 'n'; buffer[++i] = 'p';

        _base_UM7LT um7;

        _base_UM7LT::Packet packets[5];

        uint8_t numberOfPackets = um7.parseData(buffer, 90, packets, 5);


        cout << endl << "Data length: " << int(buffer[3]) << endl;
        writeArray(&(buffer[5]),buffer[3]);

        showPacket(packets[0]);
    }


    void parseDataFromCOM() {
        PortCOM com(4,115200);
        _base_UM7LT::Packet packets[10];
        _base_UM7LT um7;

        uint8_t buffer[251];

        com.open();

        while (true) {
            uint8_t read = com.readBlock(buffer, 250);
            if (read > 0) {
//                cout<<int(read) << endl;
                uint8_t parsedPackets = um7.parseData(buffer, read, packets, 10);
                for (int i = 0; i < parsedPackets; ++i) {
                    showPacket(packets[i]);
                }
            }
        }
    }

}



#endif // _base_UM7LT_UNIT_TESTS