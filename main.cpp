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
#include <CONFIG.h>
#include <iomanip>
#include <fstream>
#include <conio.h>
#include <complex>
#include <MovingArray.h>

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
//    cout << m.getRows() << ", " << m.getColumns() << endl << endl;
}

void test(UM7_LT_packet const &p){
    cout << "main:   " << hex;
    for (int i = 0; i < p.length; ++i) {
        cout << " " << int(p.data[i]);
    }
    cout << dec << endl;
}


template <typename T>
Vector3D<T> eliminateGravity(Vector3D<T> const &accelData, Vector3D<T> const &eulerAngles) {
    Vector3D<T> out;

    T tmp[3];
    tmp[0] = accelData(0);
    tmp[1] = cos(eulerAngles(0))*accelData(1) - sin(eulerAngles(0))*accelData(2);
    tmp[2] = sin(eulerAngles(0))*accelData(1) + cos(eulerAngles(0))*accelData(2);

    out(0) = cos(eulerAngles(1))*tmp[0] + sin(eulerAngles(1))*tmp[2];
    out(1) = tmp[1];
    out(2) = -1*sin(eulerAngles(1))*tmp[0] + cos(eulerAngles(1))*tmp[2];

    out(2) += GRAVITY_CONSTANT;

//    cout << " \t" << setprecision(7) << out(0) << ", \t" << out(1) << ", \t" << out(2) << ", \t" << out.norm() <<  "                      " << "\r";
    return out;
}

#include "lin_high_pass_filter.h"
double FIRfilter(double const &in) {
    static list<double> lastValues;
    lastValues.push_front(in);
    double actual = 0;
    if (lastValues.size() >= 246) {
        int i = 0;
        list<double>::iterator it;
        for (it = lastValues.begin(); it != lastValues.end(); ++it, ++i){
            actual += (*it) * B[i];
        }
        lastValues.pop_back();
    }
    return actual;
}


void UM7LTtestfunc() {

    ofstream output("tmp.csv",ios::trunc);
    output << "time,ax,ay,az\n";

    UM7LT imu(4);

    imu.turnOnThreadedRead();

    static double velocity = 0.0;
    bool turnOn = true;
    while(turnOn) {

        if (_kbhit()){
            if (_getch() == 'q') turnOn = false;
        }

        uint16_t inf = imu.getNewData();
//        if (inf & EULER) {
//            cout << "euler: " << imu.eulerAngles.back().time() << ":   \t";
//            test(imu.eulerAngles.back());
//        }
//        if (inf & QUAT) {
//            cout << "quat:  " << imu.quaternions.back().time() << ":   \t";
//            test(imu.quaternions.back());
//        }
//        if (inf & ACC) {
//            cout << "accel: " << imu.accelerometer.back().time() << ":   \t";
//            test(imu.accelerometer.back());
//        }
//        if (inf & GYRO) {
//            cout << "gyro:  " << imu.gyroscope.back().time() << ":   \t";
//            test(imu.gyroscope.back());
//        }
//        if (inf & MAG) {
//            cout << "mag:   " << imu.magnetometer.back().time() << ":   \t";
//            test(imu.magnetometer.back());
//        }
        if ((inf & EULER) && !(imu.eulerAngles.empty() || imu.accelerometer.empty()) && imu.eulerAngles.back().time() == imu.accelerometer.back().time()) { //imu.accelerometer.size() == imu.eulerAngles.size()) {
            Vector3D<double> acc = eliminateGravity(imu.accelerometer.back(), imu.eulerAngles.back());

            double norm = FIRfilter(acc.norm());

            velocity += norm*0.02;
//            if (imu.accelerometer.size() == 1)
//                velocity = acc.norm()*0.02;
//            else
//                velocity += acc.norm()*0.02;
            cout << "velocity [m/s]: " << norm << "               " << '\r';

            output << setprecision(9) << imu.accelerometer.back().time() << "," << acc(0) << "," << acc(1) << "," << acc(2) << endl;
        }

        if (inf) {
//            cout << endl;
        }
    }


}


void MovingArrayTestfunc() {
    MovingArray<int> movingArray(4);
    MovingArray<int>::iterator it(movingArray);
    MovingArray<int> movingArray1(6);

    for (int i = 0; i < movingArray.size(); ++i) {
        cout << movingArray[i] << " ";
    }
    cout << endl;

    it.value() = 43;

    it -= 3;

    it.value() = 54;

    it = movingArray1;
    (++it).value() = 654;
    MovingArray<int>::iterator it2(it);
    (it2-5).value() = 54;


    for (int i = 0; i < movingArray.size(); ++i) {
        cout << movingArray[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < movingArray1.size(); ++i) {
        cout << movingArray1[i] << " ";
    }
    cout << endl;
}


int main(){

    MovingArray<int> movingArray(5);
    MovingArray<int>::iterator it(movingArray);

    int test = 0;

    while (true) {

        test += 2;

        (++it).value() = test;

        cout << (it+0).value() << " " << (it+1).value() << " " << (it+2).value() << " " << (it+3).value() << " " << (it+4).value() << endl;

//        for (int i = 0; i < movingArray.size(); ++i) {
//            cout << movingArray[i] << " ";
//        }
//        cout << endl;

        SLEEP_MS(1000);
    }



    return 0;
}
