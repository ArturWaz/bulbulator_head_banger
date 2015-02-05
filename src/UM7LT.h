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

class Accelerometer {
    double ax;
    double ay;
    double az;
    double time;
public:
    inline double getAx() const { return ax; }
    inline void setAx(const double& ax) { Accelerometer::ax = ax; }

    inline double getAy() const { return ay; }
    inline void setAy(const double& ay) { Accelerometer::ay = ay; }

    inline double getAz() const { return az; }
    inline void setAz(const double& az) { Accelerometer::az = az; }

    inline double getTime() const { return time; }
    inline void setTime(const double& time) { Accelerometer::time = time; }
};

struct Packet {
    std::array<uint8_t,256> packet;
    void convertToChar(char*pointer)const;
    bool compareToString (const char *string, int numberOfCharacters)const;
};


class UM7_LT {

    PortCOM portCOM;
    std::thread readDataThread;
    std::thread parseDataThread;

    std::list<EulerAnglesTime> eulerList;
    std::list<Accelerometer> accelerometerList;
    double timeFrame;

    friend void readData(UM7_LT*);
    friend void parseDataPackets(UM7_LT*);

    void parseNMEApacket(const Packet &packet);
    void parseBinaryPacket(const Packet &packet);

public:

	UM7_LT(int portNumber);
	~UM7_LT();

    inline double getTimeFrame() const { return timeFrame; }
    inline void setTimeFrame(const double& timeFrame) { UM7_LT::timeFrame = timeFrame; }

    void threadedReading();

};


#endif /* UM7LT_H_ */
