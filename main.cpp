/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include "winsock2.h"
#include <iostream>
#include <io.h>
#include "serial/PortCOM.h"
#include "base_UM7LT.h"
#include "UM7LT.h"
#include "serial/ClientTCP.h"
#include "Buffer.h"
#include "SensorsStructures.h"
#include "Plot.h"



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


    UM7LT um7(3);
    Plot plot;
    plot.show();

    cout << hex;

    while (true) {
        uint16_t read = um7.getData();
        if (read & UM7LT::Data::ACC_PROC) {
            Vector3DTime &tmp = GlobalData::AccelProcessed::buffer.last();
            //cout << "Read: accel; ";
//            cout << tmp.time << "   ";
            plot.addValue(tmp(0));
        }
        if (read & UM7LT::Data::EULER) {
            Vector3DTime &tmp = GlobalData::EulerAngle::buffer.last();
            //cout << "Read: euler: ";
//            cout << tmp(0) << " " << tmp(1) << " " << tmp(2) << " " << tmp.time;
//            cout << tmp.time << "   ";
        }

        if (read) {
            //cout << endl;
        }
    }



    return 0;
}
