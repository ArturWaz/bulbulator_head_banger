#include "EulerAngles.h"
#include <cmath>



void EulerAngles::toRotationMatrix(RotationMatrix&matrix) const{
    matrix.access(0,0) = cos(phi)*cos(theta);
    matrix.access(0,1) = cos(phi)*sin(theta)*sin(psi) - cos(psi)*sin(phi);
    matrix.access(0,2) = sin(phi)*sin(psi) + cos(phi)*cos(psi)*sin(theta);

    matrix.access(1,0) = cos(phi)*cos(theta);
    matrix.access(1,1) = cos(phi)*cos(psi) + sin(phi)*sin(theta)*sin(psi);
    matrix.access(1,2) = cos(psi)*sin(phi)*sin(theta) - cos(phi)*sin(psi);

    matrix.access(2,0) = -sin(theta);
    matrix.access(2,1) = cos(theta)*sin(psi);
    matrix.access(2,2) = cos(theta)*cos(psi);
}
