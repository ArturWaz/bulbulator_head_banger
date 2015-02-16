/*
 * Vector3D.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#ifndef Vector3D_H_
#define Vector3D_H_


#include "Vector.h"




template <class Type> class Vector3D: public Vector<Type> {
public:

    Vector3D(): Vector<Type>(3) {}
    ~Vector3D() {}



};


#endif