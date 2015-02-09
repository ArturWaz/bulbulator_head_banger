#include "RotationMatrix.h"
#include <cstring>


RotationMatrix::RotationMatrix(RotationMatrix const &aConst): matrixPointer(matrix[0]) {
    std::memcpy(matrixPointer,aConst.matrixPointer,9* sizeof(double));
}

RotationMatrix &RotationMatrix::operator=(const RotationMatrix&matrix) {
    std::memcpy(matrixPointer,matrix.matrixPointer,9*sizeof(double));
    return *this;
}

