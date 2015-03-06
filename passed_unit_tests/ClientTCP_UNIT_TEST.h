/*
 * ClientTCP_UNIT_TEST.h
 *
 *  Created on: 3/5/2015
 *      Author: Artur
 */


#ifndef ClientTCP_UNIT_TEST_H_
#define ClientTCP_UNIT_TEST_H_


#include "ClientTCP.h"


namespace ClientTCP_UNIT_TEST {
    using namespace std;

    void coutData(uint8_t const *array, uint8_t const length) {
        cout << hex;
        for (int i = 0; i < length; ++i) {
            cout << int(array[i]) << " ";
        }
        cout << dec << endl;
    }

    void readBlock() {
        ClientTCP client("localhost","12000");
        uint8_t buffer[250];

        client.connect();
        while (true) {
            uint8_t read = client.readBlock(buffer, 30);
            coutData(buffer, read);
        }

    }


}

#endif //__ClientTCP_UNIT_TEST_H_