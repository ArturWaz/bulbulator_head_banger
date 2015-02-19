/*
 * RotationMatrix.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#ifndef RotationMatrix_H_
#define RotationMatrix_H_


#include <CONFIG.h>
#include "Matrix.h"
#include "myMath.h"



class RotationMatrix: public Matrix<double> {


public:

    enum RotationAxis { rotX=0, rotY, rotZ};


    RotationMatrix(): Matrix(3,3) {}
    RotationMatrix(RotationAxis r, double const &angle): Matrix(3,3) {
        if (r == rotX) {
            Matrix::operator()(0,0) = 1; Matrix::operator()(0,1) = 0;          Matrix::operator()(0,2) = 0;
            Matrix::operator()(1,0) = 0; Matrix::operator()(1,1) = cos(angle); Matrix::operator()(1,2) = -sin(angle);
            Matrix::operator()(2,0) = 0; Matrix::operator()(2,1) = sin(angle); Matrix::operator()(2,2) = cos(angle);
        }
        else if (r == rotY) {
            Matrix::operator()(0,0) = cos(angle);  Matrix::operator()(0,1) = 0; Matrix::operator()(0,2) = sin(angle);
            Matrix::operator()(1,0) = 0;           Matrix::operator()(1,1) = 1; Matrix::operator()(1,2) = 0;
            Matrix::operator()(2,0) = -sin(angle); Matrix::operator()(2,1) = 0; Matrix::operator()(2,2) = cos(angle);
        }
        else if (r == rotZ) {
            Matrix::operator()(0,0) = cos(angle); Matrix::operator()(0,1) = -sin(angle); Matrix::operator()(0,2) = 0;
            Matrix::operator()(1,0) = sin(angle); Matrix::operator()(1,1) = cos(angle);  Matrix::operator()(1,2) = 0;
            Matrix::operator()(2,0) = 0;          Matrix::operator()(2,1) = 0;           Matrix::operator()(2,2) = 1;
        }
        else
            throw -1;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (fabs(Matrix::operator()(i,j)) < EPSILON) Matrix::operator()(i,j) = 0;
            }
        }
    }
    ~RotationMatrix() {}


};


#endif