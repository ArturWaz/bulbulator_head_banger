/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include "winsock2.h"
#include <iostream>
#include <fstream>
#include <io.h>
#include <CONFIG.h>
#include <conio.h>
#include "serial/PortCOM.h"
#include "base_UM7LT.h"
#include "UM7LT.h"
#include "serial/ClientTCP.h"
#include "Buffer.h"
#include "SensorsStructures.h"
#include "Plot.h"
#include "DigitalFilter.h"



using namespace std;


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

    //out(2) += GRAVITY_CONSTANT;

//    cout << " \t" << setprecision(7) << out(0) << ", \t" << out(1) << ", \t" << out(2) << ", \t" << out.norm() <<  "                      " << "\r";
    return out;
}



#define TIME_INTERVAL 25       // number of samples
double detectMove(double const &actualValue, double const &lastValue, double const &frequency) {
    double out = 0.0;

    static double integral(0.0);
    static Buffer<double> buffer(TIME_INTERVAL);

    double derivative = (actualValue-lastValue)*frequency;
    buffer.push((fabs(derivative) < 0.5) ? 0.0 : derivative);

    double integralTMP = 0.0;
    Buffer<double>::iterator it = buffer.begin();
    for (size_t j = 0; j < buffer.size(); ++j) {
        integralTMP += *it;
        ++it;
    }

    if (fabs(integralTMP) < 1) {
        integralTMP = 0.0;
        it = buffer.begin();
        for (size_t j = 0; j < buffer.size(); ++j) {
            integralTMP += fabs(*it);
            ++it;
        }
        integral += integralTMP;
//        out = integral;
    }
    out = integral;
    return out;
}


int main(){

    UM7LT um7(4);
    Plot plot;
    plot.show();

//    std::ofstream outputFile("tmp.csv",ios::trunc);
//    outputFile << "time,ax,ay,az\n";

    cout << hex;

    DigitalFilter::Average<double,500> average;
    size_t average_tmp_value_for_iter = 0;
    double bias = 0.0;

    double lastValue = 0.0;
    double integral = 0.0;

    bool quit = false;
    while (!quit) {

        if (_kbhit())
            if (_getch() == 'q')
                quit = true;

        uint16_t read = um7.getData();
        if (read & UM7LT::Data::ACC_RAW) {
            Vector3DTime &tmp = GlobalData::AccelRaw::buffer.last();
            //cout << "Read: euler: ";
//            double actualInputValue = tmp(0);
//
//            double derivative = (actualInputValue - lastInputValue)*50;
//            integral += (actualInputValue - lastInputValue);
//
//            double actualOutValue = derivative;
//
//            plot.addValue(actualOutValue);
//            cout << actualOutValue << endl;
//            lastInputValue = actualInputValue;
        }
        if (read & UM7LT::Data::ACC_PROC) {
            Vector3DTime &tmp = GlobalData::AccelProcessed::buffer.last();
            //cout << "Read: accel; ";
//            cout << tmp.time << "   ";
            //plot.addValue(tmp(0));
        }
        if (read & UM7LT::Data::EULER) {
            Vector3DTime &tmp = GlobalData::EulerAngle::buffer.last();
            //cout << "Read: euler: ";
//            cout << tmp(0) << " " << tmp(1) << " " << tmp(2) << " " << tmp.time;
//            cout << tmp.time << "   ";
        }

        if (read & UM7LT::Data::ACC_PROC) {//(read & UM7LT::Data::EULER && (GlobalData::AccelProcessed::buffer.last().time == GlobalData::EulerAngle::buffer.last().time)) {
            Vector3D<double> accel = GlobalData::AccelProcessed::buffer.last();
//            Vector3D<double> accel(eliminateGravity(GlobalData::AccelProcessed::buffer(0), GlobalData::EulerAngle::buffer.last()));


            double actualInputValue = accel(0);


//            double actualOutValue = actualInputValue - bias;

//            double derivative = (actualInputValue - lastValue)*200;

//            if (fabs(actualOutValue) < 0.02) actualOutValue = 0;

//            average.filter(actualOutValue);
//            if (average_tmp_value_for_iter == 500)
//                bias = average.actual();
//            else if (average_tmp_value_for_iter > 501)
//                integral += actualOutValue/50;


            double whatToShow = detectMove(actualInputValue, lastValue, 200);
            plot.addValue(whatToShow);
            cout << whatToShow << endl;
//            outputFile << GlobalData::AccelProcessed::buffer.last().time << "," << whatToShow << endl;

            lastValue = actualInputValue;
            ++average_tmp_value_for_iter;
        }

        if (read) {
            //cout << endl;
        }
    }

//    outputFile.close();

    return 0;
}
