#include "Quaternion.h"
#include <cstring>
#include <cmath>



Quaternion &Quaternion::operator=(Quaternion const &aConst) {
    std::memcpy(vector,aConst.vector,4*sizeof(double));
    return *this;
}

double Quaternion::norm() const {
    return sqrt(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2] + vector[3]*vector[3]);
}

void Quaternion::toUnit(){
    double n = norm();
    vector[0] /= n;
    vector[1] /= n;
    vector[2] /= n;
    vector[3] /= n;
}

void Quaternion::toRotationMatrix(RotationMatrix &aConst) const{
    aConst(0,0) = vector[0]*vector[0] + vector[1]*vector[1] - vector[2]*vector[2] - vector[3]*vector[3];
    aConst(0,1) = 2*(vector[1]*vector[2] - vector[0]*vector[3]);
    aConst(0,2) = 2*(vector[1]*vector[3] + vector[0]*vector[2]);

    aConst(1,0) = 2*(vector[1]*vector[2] + vector[0]*vector[3]);
    aConst(1,1) = vector[0]*vector[0] - vector[1]*vector[1] + vector[2]*vector[2] - vector[3]*vector[3];
    aConst(1,2) = 2*(vector[2]*vector[3] - vector[0]*vector[1]);

    aConst(2,0) = 2*(vector[1]*vector[3] - vector[0]*vector[2]);
    aConst(2,1) = 2*(vector[2]*vector[3] + vector[0]*vector[1]);
    aConst(2,2) = vector[0]*vector[0] - vector[1]*vector[1] - vector[2]*vector[2] + vector[3]*vector[3];
}

Quaternion &Quaternion::operator*(Quaternion const &aConst) {
    double temp0, temp1, temp2, temp3;
    temp0 = vector[0]*aConst[0] - vector[1]*aConst[1] - vector[2]*aConst[2] - vector[3]*aConst[3];
    temp1 = vector[0]*aConst[1] + vector[1]*aConst[0] + vector[2]*aConst[3] - vector[3]*aConst[2];
    temp2 = vector[0]*aConst[2] - vector[1]*aConst[3] + vector[2]*aConst[0] + vector[3]*aConst[1];
    temp3 = vector[0]*aConst[3] + vector[1]*aConst[2] - vector[2]*aConst[1] + vector[3]*aConst[0];
    vector[0] = temp0; vector[1] = temp1; vector[2] = temp2; vector[3] = temp3;
    return *this;
}
