/*
 * UM7LT.h
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */

#ifndef UM7LT_H_
#define UM7LT_H_

#include <math.h>
#include <list>
#include <thread>
#include "PortCOM.h"
#include "Quaternion.h"
#include "EulerAngles.h"


class QuaternionTime: public Quaternion{
    double time;
public:
    QuaternionTime(): time(0.0) {}
    QuaternionTime(const double& a, const double& b, const double& c, const double& d, const double& time):
            Quaternion(a,b,c,d), time(time) {}
    inline double getTime() const { return time; }
    inline void setTime(const double& time) { QuaternionTime::time = time; }
};

class EulerAnglesTime: public EulerAngles{
    double time;
public:
    EulerAnglesTime(): time(0.0) {}
    EulerAnglesTime(const double& phi, const double& theta, const double& psi, const double& time):
            EulerAngles(phi,theta,psi), time(time) {}
    inline double getTime() const { return time; }
    inline void setTime(const double& time) { EulerAnglesTime::time = time; }
};


class UM7_LT {

    PortCOM portCOM;
    std::thread readDataThread;

    std::list<EulerAnglesTime> eulerList;

    void readData();

public:

	UM7_LT(int portNumber);
	~UM7_LT();

    void threadedReading();

};


#endif /* UM7LT_H_ */
