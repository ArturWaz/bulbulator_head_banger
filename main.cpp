/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include <iostream>
#include "PortCOM.h"
#include "base_UM7LT.h"
#include "base_UM7LT_UNIT_TESTS.h"

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

    _base_UM7LT_UNIT_TESTS::parseDataFromCOM();


    return 0;
}
