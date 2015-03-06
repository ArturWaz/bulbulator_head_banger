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


struct Vector3DTime : public Vector3D {
    double time;
};

struct QuaternionTime : public Quaternion {
    double time;
};


#endif //__SensorsStructures_H_