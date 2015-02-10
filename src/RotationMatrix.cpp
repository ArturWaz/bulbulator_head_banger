#include "RotationMatrix.h"
#include <cstring>
#include <cmath>


RotationMatrix::RotationMatrix(RotationMatrix const &aConst): matrixPointer(matrix[0]) {
    std::memcpy(matrixPointer,aConst.matrixPointer,9* sizeof(double));
}

RotationMatrix &RotationMatrix::operator=(const RotationMatrix&matrix) {
    std::memcpy(matrixPointer,matrix.matrixPointer,9*sizeof(double));
    return *this;
}

RotationMatrix RotationMatrix::operator*(RotationMatrix const &aConst) {
    RotationMatrix rotationMatrix;
    rotationMatrix[0*3+0] = matrixPointer[0]*aConst.matrixPointer[0] + matrixPointer[1]*aConst.matrixPointer[3] + matrixPointer[2]*aConst.matrixPointer[6];
    rotationMatrix[0*3+1] = matrixPointer[0]*aConst.matrixPointer[1] + matrixPointer[1]*aConst.matrixPointer[4] + matrixPointer[2]*aConst.matrixPointer[7];
    rotationMatrix[0*3+2] = matrixPointer[0]*aConst.matrixPointer[2] + matrixPointer[1]*aConst.matrixPointer[5] + matrixPointer[2]*aConst.matrixPointer[8];

    rotationMatrix[1*3+0] = matrixPointer[3]*aConst.matrixPointer[0] + matrixPointer[4]*aConst.matrixPointer[3] + matrixPointer[5]*aConst.matrixPointer[6];
    rotationMatrix[1*3+1] = matrixPointer[3]*aConst.matrixPointer[1] + matrixPointer[4]*aConst.matrixPointer[4] + matrixPointer[5]*aConst.matrixPointer[7];
    rotationMatrix[1*3+2] = matrixPointer[3]*aConst.matrixPointer[2] + matrixPointer[4]*aConst.matrixPointer[5] + matrixPointer[5]*aConst.matrixPointer[8];

    rotationMatrix[2*3+0] = matrixPointer[6]*aConst.matrixPointer[0] + matrixPointer[7]*aConst.matrixPointer[3] + matrixPointer[8]*aConst.matrixPointer[6];
    rotationMatrix[2*3+1] = matrixPointer[6]*aConst.matrixPointer[1] + matrixPointer[7]*aConst.matrixPointer[4] + matrixPointer[8]*aConst.matrixPointer[7];
    rotationMatrix[2*3+2] = matrixPointer[6]*aConst.matrixPointer[2] + matrixPointer[7]*aConst.matrixPointer[5] + matrixPointer[8]*aConst.matrixPointer[8];
    return rotationMatrix;
}

void RotationMatrix::xRotation(const double &angle) {
    matrix[0][0] = 1;   matrix[0][1] = 0;   matrix[0][2] = 0;
    matrix[1][0] = 0;   matrix[1][1] = cos(angle);   matrix[1][2] = -sin(angle);
    matrix[2][0] = 0;   matrix[2][1] = sin(angle);   matrix[2][2] = cos(angle);
}

void RotationMatrix::yRotation(const double &angle) {
    matrix[0][0] = cos(angle);   matrix[0][1] = 0;   matrix[0][2] = sin(angle);
    matrix[1][0] = 0;   matrix[1][1] = 1;   matrix[1][2] = 0;
    matrix[2][0] = -sin(angle);   matrix[2][1] = 0;   matrix[2][2] = cos(angle);
}

void RotationMatrix::zRotation(const double &angle) {
    matrix[0][0] = cos(angle);   matrix[0][1] = -sin(angle);   matrix[0][2] = 0;
    matrix[1][0] = sin(angle);   matrix[1][1] = cos(angle);   matrix[1][2] = 0;
    matrix[2][0] = 0;   matrix[2][1] = 0;   matrix[2][2] = 1;
}
