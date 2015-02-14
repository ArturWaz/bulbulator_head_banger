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
#include <mutex>
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

    void eleminitateGravity(EulerAnglesTime);
    void eleminitateGravity(QuaternionTime);
};

class Gyroscope {
    double gx;
    double gy;
    double gz;
    double time;
public:
    inline double getGx() const { return gx; }
    inline void setGx(const double& ax) { Gyroscope::gx = ax; }

    inline double getGy() const { return gy; }
    inline void setGy(const double& ay) { Gyroscope::gy = ay; }

    inline double getGz() const { return gz; }
    inline void setGz(const double& az) { Gyroscope::gz = az; }

    inline double getTime() const { return time; }
    inline void setTime(const double& time) { Gyroscope::time = time; }
};

struct Packet {
    std::array<uint8_t,256> packet;
    int packetLength;
    void coutPacket(const char*string)const;
    void convertToChar(char*pointer)const;
    bool compareToString (const char *string, int numberOfCharacters)const;
    inline uint8_t operator[](int i) const { return packet[i]; }
};



class UM7_LT: private PortCOM {

    std::thread readDataThread;
    std::thread parseDataThread;

    std::list<EulerAnglesTime> eulerList;
    std::mutex eulerListMutex;
    std::list<QuaternionTime> quaternionList;
    std::mutex quaternionListMutex;
    std::list<Accelerometer> accelerometerList;
    std::mutex accelerometerListMutex;
    std::list<Gyroscope> gyroList;
    std::mutex gyroListMutex;
    double timeFrame;

    friend void readData(UM7_LT*);
    friend void parseDataPackets(UM7_LT*);

    void parseNMEApacket(const Packet &);
    void parseBinaryPacket(const Packet &);

public:

	UM7_LT(int portNumber);
	~UM7_LT();

    inline double getTimeFrame() const { return timeFrame; }
    inline void setTimeFrame(const double& timeFrame) { UM7_LT::timeFrame = timeFrame; }

    void threadedReading();


};


#endif /* UM7LT_H_ */
