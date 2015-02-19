/*
 * UM7LT.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#include <list>
#include <Vector.h>
#include "_base_UM7LT.h"
#include "Vector3D.h"
#include "Quaternion.h"

#ifndef UM7LT_H_
#define UM7LT_H_



enum LastAcquiredData {
    NONE    = 0x0000,
    GYRO    = 0x0001,
    ACC     = 0x0002,
    QUAT    = 0x0004,
    EULER   = 0x0008,
    MAG     = 0x0010
};



class TimeVector3D: public Vector3D<double> {

    double Time;

public:

    TimeVector3D(): Time(0.0) {}
    TimeVector3D(double x, double y, double z, double time): Time(time) {
        Vector::operator()(0) = x;
        Vector::operator()(1) = y;
        Vector::operator()(2) = z;
    }
    TimeVector3D(TimeVector3D const &v): Vector3D(static_cast<const Vector3D<double>&>(v)), Time(v.Time) {}

    inline TimeVector3D const &operator=(TimeVector3D const &v) {
        Vector::operator=(static_cast<const Vector3D<double>&>(v));
        Time = v.Time;
    }

    inline double const &time() const { return Time; }
    inline double &time() { return Time; }

};



class TimeQuaternion: public Quaternion {

    double Time;

public:

    TimeQuaternion(): Time(0.0) {}
    TimeQuaternion(double a, double b, double c, double d, double time): Time(time) {
        Vector::operator()(0) = a;
        Vector::operator()(1) = b;
        Vector::operator()(2) = c;
        Vector::operator()(3) = d;
    }
    TimeQuaternion(TimeQuaternion const &v): Quaternion(static_cast<const Quaternion&>(v)), Time(v.Time) {}

    inline TimeQuaternion const &operator=(TimeQuaternion const &v) {
        Vector::operator=(static_cast<const Quaternion&>(v));
        Time = v.Time;
    }

    inline double const &time() const { return Time; }
    inline double &time() { return Time; }

};




class UM7LT: public _base_UM7LT {
public:

    std::list<TimeVector3D> accelerometer;       // [m/s/s]
    std::list<TimeVector3D> gyroscope;           // [rad/s]
    std::list<TimeVector3D> magnetometer;        //  ???

    std::list<TimeVector3D> eulerAngles;         // [rad]
    std::list<TimeQuaternion> quaternions;       //  ---



    UM7LT(int portNumber);
    ~UM7LT();


    uint16_t getNewData();


};


#endif