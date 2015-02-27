/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include <iostream>
#include "_base_UM7LT.h"
#include <math/Vector.h>
#include <math/Matrix.h>
#include <DefineFunctions.h>
#include <math/RotationMatrix.h>
#include <UM7LT.h>
#include <CONFIG.h>
#include <cstdint>
#include <iomanip>
#include <fstream>
#include <complex>
#include <DigitalFilter.h>
#include <conio.h>
#include "Buffer.h"

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



double calculateBias(double const &in) {
    double actual = 0.0;
    static list<double> lastValues;

//    if (lastValues.size() < 300)
        lastValues.push_front(in);
    if (lastValues.size() >= 300) {
        list<double>::iterator it;
        for (it = lastValues.begin(); it != lastValues.end(); ++it) {
            actual += *it;
        }
        actual /= lastValues.size();

        lastValues.pop_back();
    }

    return actual;
}


double medianFilter(double const &in) {
    static list<double> lastValues;
    lastValues.push_front(in);
    double actual = 0.0;
    if (lastValues.size() >= 10) {
        lastValues.sort();
        list<double>::iterator it = lastValues.begin();
        int medIndex = int(lastValues.size()/2)+1;
        for (int i = 0; i < medIndex; ++i) ++it;
        actual = *it;
        lastValues.pop_back();
    }
    return actual;
}


void UM7LTtestfunc() {

    ofstream output("tmp.csv",ios::trunc);
    output << "time,ax,ay,az\n";

    UM7LT imu(4);

    imu.turnOnThreadedRead();

    DigitalFilter::IIR_DirectFormII<double,3> filter;
    DigitalFilter::Median<double,500> median;

    static double velocity = 0.0;
    static double position = 0.0;
    static double lastValue = 0.0;

    bool turnOn = true;
    while(turnOn) {

//        if (_kbhit()){
//            if (_getch() == 'q') turnOn = false;
//        }

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
        if ((inf & EULER) && !(imu.eulerAngles.size() < 10 || imu.accelerometer.empty()) && imu.eulerAngles.back().time() == imu.accelerometer.back().time()) { //imu.accelerometer.size() == imu.eulerAngles.size()) {

            list<TimeVector3D>::iterator iterator1 = imu.eulerAngles.end();
            for (int i = 0; i < 1; ++i) --iterator1;

            Vector3D<double> acc = eliminateGravity(imu.accelerometer.back(), *iterator1);

            double actualValue = acc(0);

            double bias = 0.2;//median.filter(actualValue);
            double accel = bias!=0.0? actualValue-bias:0.0;

            const double precisionAccel = 0.0;
            const double precisionVelo = 0.0;

            velocity += fabs(accel) > precisionAccel ? accel*0.02 : 0.0;
            position += fabs(velocity) > precisionVelo ? velocity*0.02 : 0.0;

//            if (imu.accelerometer.size() == 1)
//                velocity = acc.norm()*0.02;
//            else
//                velocity += acc.norm()*0.02;

            cout << fixed << setprecision(3) << "pose [m]: " << setw(8) << position << ",    " << "velocity [m/s]: " << setw(8) << velocity << ",    " << "acc [m/s/s]: " << setprecision(3) << setw(8) << (fabs(accel)> precisionAccel ? accel *0.02:0.0) << " " << '\r';
//            cout << "acc [m/s]: " << setw(30) << fixed << setprecision(9) << actualValue-bias << "               " << '\r';

//            output << setprecision(9) << imu.accelerometer.back().time() << "," << acc(0) << "," << acc(1) << "," << acc(2) << endl;
            lastValue = actualValue;
        }

        if (inf) {
//            cout << endl;
        }
    }


}




void MovingArrayTestfunc() {

    Buffer<int,5> movingArray;

    int test = 0;

    while (true) {

        test += 2;

        movingArray.push(test);
        cout << movingArray(0) << " " << movingArray(1) << " " << movingArray(2) << " " << movingArray(3) << " " << movingArray(4) << endl;


        SLEEP_MS(1000);
    }

}



void digitalFilterTest() {

    DigitalFilter::Median<double,155> median;
    DigitalFilter::Average<double,100> average;
    DigitalFilter::IIR_DirectFormII<double,3> filter;

    double x = 0;

    const double unit = 1;

    bool impulse = false;

    while (true) {

        if (_kbhit()) {
            char c = getch();
            if (c == 'q') return;
            if (c == 'i') x += unit;
            if (c == 'd') x -= unit;
            if (c == 'p') { x += unit; impulse = true; }
        }

        double filtered = filter.filterMatlabCoeffs(x);
        double bias = median.filter(filtered);

        cout << fixed << setprecision(9) << setw(30) << filtered << " " << bias << " " << filtered - bias << endl;

        if (impulse) {
            x = 0;
            impulse = false;
        }

        SLEEP_MS(50);
    }

}


int main(){

    UM7LTtestfunc();

    return 0;
}
