/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include <iostream>
#include <UM7_LT.h>
#include <Vector.h>

using namespace std;

template <typename T>
void test(Vector<T>&v){
    cout << v[0] << ", " << v[1] << ", " << v[2] << endl;
    cout << v.getNumberOfRows() << ", " << v.getNumberOfColumns() << endl << endl;
}


int main(){


    Vector<double> v(3);

    v[0] = 56;
    v[1] = 34;
    v[2] = 89;
    test(v);

    Vector<double> v1(3);

    v1 = v;

    v1 = (v1+v).transpose();
    test(v1);

    v1 = v1/2;
    test(v1);

//    UM7_LT test(16);
//
//    UM7_LT_packet lol;
//
//    test.turnOnThreadedRead();
//    while (true){
//        test.takeLastPacket(lol);
//    }

    return 0;
}
