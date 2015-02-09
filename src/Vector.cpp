#include "Vector.h"
#include <cstring>


Vector::Vector(int length): length(length) {
    vector = new double[length];
}

Vector::Vector(Vector const &aConst): length(aConst.length) {
    std::memcpy(this->vector,aConst.vector,length*sizeof(double));
}

Vector::~Vector() {
    delete[] vector;
}
