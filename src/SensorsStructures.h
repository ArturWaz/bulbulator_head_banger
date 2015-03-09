/*
 * SensorsStructures.h
 *
 *  Created on: 3/6/2015
 *      Author: Artur
 */


#ifndef SensorsStructures_H_
#define SensorsStructures_H_


#include "math/Vector3D.h"
#include "math/Quaternion.h"


struct Vector3DTime : public Vector3D<double> {
    double time;
    Vector3DTime(): time(0.0) {}
    Vector3DTime(double const &x, double const &y, double const &z, double const &time): time(time) {
        Vector3D::operator()(0) = x;
        Vector3D::operator()(1) = y;
        Vector3D::operator()(2) = z;
    }
};

struct QuaternionTime : public Quaternion {
    double time;
};


#endif //__SensorsStructures_H_