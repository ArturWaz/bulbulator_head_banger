/*
 * RotationMatrix.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#ifndef RotationMatrix_H_
#define RotationMatrix_H_




class RotationMatrix: public Matrix<double> {


public:

    RotationMatrix(): Matrix(3,3) {}
    ~RotationMatrix() {}



};


#endif