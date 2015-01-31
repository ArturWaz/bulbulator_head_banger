/*
 * EulerAngles.h
 *
 *  Created on: 31.01.15
 *      Author: artur
 */


#ifndef EulerAngles_H_
#define EulerAngles_H_



class EulerAngles {

    double phi;
    double theta;
    double psi;

public:

    EulerAngles(): phi(0.0), theta(0.0), psi(0.0) {}
    EulerAngles(const double& phi, const double& theta, const double& psi):
            phi(phi), theta(theta), psi(psi) {}

    inline double getPhi() const { return phi; }
    inline void setPhi(const double& phi) { EulerAngles::phi = phi; }

    inline double getTheta() const { return theta; }
    inline void setTheta(const double& theta) { EulerAngles::theta = theta; }

    inline double getPsi() const { return psi; }
    inline void setPsi(const double& psi) { EulerAngles::psi = psi; }
};


#endif //__EulerAngles_H_