/*
 * Quaternion.h
 *
 *  Created on: 31.01.15
 *      Author: artur
 */


#ifndef Quaternion_H_
#define Quaternion_H_


class Quaternion {

    double a;
    double b;
    double c;
    double d;

public:

    Quaternion(): a(0.0), b(0.0), c(0.0), d(0.0) {}
    Quaternion(const double& a, const double& b, const double& c,const double& d):
            a(a), b(b), c(c), d(d) {}

    inline double getA() const { return a; }
    inline void setA(const double& a) { Quaternion::a = a; }

    inline double getB() const { return b; }
    inline void setB(const double& b) { Quaternion::b = b; }

    inline double getC() const { return c; }
    inline void setC(const double& c) { Quaternion::c = c; }

    inline double getD() const { return d; }
    inline void setD(const double& d) { Quaternion::d = d; }
};


#endif //__Quaternion_H_