/*
 * RotationMatrix.h
 *
 *  Created on: 31.01.15
 *      Author: artur
 */


#ifndef RotationMatrix_H_
#define RotationMatrix_H_


#include "Vector.h"

class RotationMatrix {

    double matrix[3][3];
    double *const matrixPointer;

public:

    RotationMatrix(): matrixPointer(matrix[0]) {}
    RotationMatrix(const RotationMatrix&);
    ~RotationMatrix() {}

    RotationMatrix&operator=(const RotationMatrix&);

    inline const double&operator[](int i) const { return matrixPointer[i]; }
    inline double&operator[](int i) { return matrixPointer[i]; }

    inline const double&access(int i, int j) const { return matrix[i][j]; }
    inline double&access(int i, int j) { return matrix[i][j]; }

};


#endif //__RotationMatrix_H_