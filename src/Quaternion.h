/*
 * Quaternion.h
 *
 *  Created on: 31.01.15
 *      Author: artur
 */


#ifndef Quaternion_H_
#define Quaternion_H_


#include "RotationMatrix.h"

class Quaternion {

    double vector[4];
    double *const a;
    double *const b;
    double *const c;
    double *const d;

public:

    Quaternion(): a(vector), b(vector+1), c(vector+2), d(vector+3) {}
    Quaternion(const double& a, const double& b, const double& c,const double& d):
            a(vector), b(vector+1), c(vector+2), d(vector+3){
        vector[0] = a; vector[1] = b; vector[2] = c; vector[3] = d;
    }

    Quaternion&operator=(const Quaternion&);

    inline double getA() const { return vector[0]; }
    inline void setA(const double& a) { vector[0] = a; }

    inline double getB() const { return vector[1]; }
    inline void setB(const double& b) { vector[1] = b; }

    inline double getC() const { return vector[2]; }
    inline void setC(const double& c) { vector[2] = c; }

    inline double getD() const { return vector[3]; }
    inline void setD(const double& d) { vector[3] = d; }

    inline const double&operator[](int i) const { return vector[i]; }
    inline double&operator[](int i) { return vector[i]; }

    Quaternion&operator*(const Quaternion&);

    double norm() const;

    void toUnit();

    void toRotationMatrix(RotationMatrix&)const;

};


#endif //__Quaternion_H_