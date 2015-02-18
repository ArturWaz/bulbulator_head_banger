/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include <iostream>
#include "_base_UM7LT.h"
#include <Vector.h>
#include <Matrix.h>
#include <DefineFunctions.h>
#include <RotationMatrix.h>
#include <UM7LT.h>

using namespace std;

template <typename T>
void test(Vector<T> &v){
    for (int i = 0; i < v.getLength(); ++i)
        cout << v[i] << ", ";
    cout << endl;
//    cout << v.getRows() << ", " << v.getColumns() << endl << endl;
}

template <typename T>
void test(Matrix<T> &m){
    for (int i = 0; i < m.getRows(); ++i) {
        for (int j = 0; j < m.getColumns(); ++j) {
            cout << m(i,j) << ", ";
        }
        cout << endl;
    }
    cout << m.getRows() << ", " << m.getColumns() << endl << endl;
}

void test(UM7_LT_packet const &p){
    cout << "main:   " << hex;
    for (int i = 0; i < p.length; ++i) {
        cout << " " << int(p.data[i]);
    }
    cout << dec << endl;
}




int main(){


    UM7LT imu(4);

    imu.turnOnThreadedRead();

    while(true) {

        uint16_t inf = imu.getNewData();
        if (inf & EULER) {
            cout << "euler: " << imu.eulerAngles.back().time() << ":   \t";
            test(imu.eulerAngles.back());
        }
        if (inf & QUAT) {
            cout << "quat:  " << imu.quaternions.back().time() << ":   \t";
            test(imu.quaternions.back());
        }
        if (inf & ACC) {
            cout << "accel: " << imu.accelerometer.back().time() << ":   \t";
            test(imu.accelerometer.back());
        }
        if (inf & GYRO) {
            cout << "gyro:  " << imu.gyroscope.back().time() << ":   \t";
            test(imu.gyroscope.back());
        }
        if (inf & MAG) {
            cout << "mag:   " << imu.magnetometer.back().time() << ":   \t";
            test(imu.magnetometer.back());
        }
        if (inf)
            cout << endl;
    }


    return 0;
}
