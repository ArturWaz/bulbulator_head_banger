/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include "winsock2.h"
#include <iostream>
#include "PortCOM.h"
#include "base_UM7LT.h"
//#include "UM7LT.h"
#include "ClientTCP.h"

using namespace std;

void writeArray (uint8_t *a, uint8_t l) {
    if (a == nullptr) { cerr << "array pointer is null\n"; return; }
    cout << hex;
    for (int i = 0; i < l; ++i) {
        cout << int(a[i]) << " ";
    }
    cout << dec << endl;
}





int main(){


    return 0;
}
