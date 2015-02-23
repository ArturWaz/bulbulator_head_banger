/*
 * Quaternion.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#ifndef Quaternion_H_
#define Quaternion_H_



#include "Vector.h"




class Quaternion: public Vector<double> {


public:

    Quaternion(): Vector(4) {}
    ~Quaternion() {}

};


#endif