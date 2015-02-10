#include "EulerAngles.h"
#include <cmath>



void EulerAngles::toRotationMatrix(RotationMatrix&matrix) const{
    matrix(0,0) = cos(phi)*cos(theta);
    matrix(0,1) = cos(phi)*sin(theta)*sin(psi) - cos(psi)*sin(phi);
    matrix(0,2) = sin(phi)*sin(psi) + cos(phi)*cos(psi)*sin(theta);

    matrix(1,0) = cos(phi)*cos(theta);
    matrix(1,1) = cos(phi)*cos(psi) + sin(phi)*sin(theta)*sin(psi);
    matrix(1,2) = cos(psi)*sin(phi)*sin(theta) - cos(phi)*sin(psi);

    matrix(2,0) = -sin(theta);
    matrix(2,1) = cos(theta)*sin(psi);
    matrix(2,2) = cos(theta)*cos(psi);
}
