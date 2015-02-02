/*
 * RobotControl.h
 *
 *  Created on: 31.01.15
 *      Author: artur
 */


#ifndef RobotControl_H_
#define RobotControl_H_

#include <cstdint>
#include <iostream>
#include "MiniMaestro.h"



// todo Make test of this class


enum LEG {
    leftFront0 = 0,
    leftFront1,
    leftFront2,
    leftRear0,
    leftRear1,
    leftRear2,
    rightRear0,
    rightRear1,
    rightRear2,
    rightFront0,
    rightFront1,
    rightFront2
};



class RobotControl {

    uint8_t _maxSpeed;
    uint8_t _minSpeed;

    uint16_t _minLegAngle0;
    uint16_t _maxLegAngle0;

    uint16_t _minLegAngle1;
    uint16_t _maxLegAngle1;

    uint16_t _minLegAngle2;
    uint16_t _maxLegAngle2;

    uint16_t _zeroLegAngle[12];

    MiniMaestro _servos;

protected:

    inline void setPositionJoint0(LEG servo, uint16_t position);
    inline void setPositionJoint1(LEG servo, uint16_t position);
    inline void setPositionJoint2(LEG servo, uint16_t position);

public:

    RobotControl(int miniMaestroPortNumber);
    ~RobotControl();

    void setZeroPosition();

    inline void setLeftFront0(int16_t position);
    inline void setRightFront0(int16_t position);
    inline void setRightRear0(int16_t position);
    inline void setLeftRear0(int16_t position);

    inline void setLeftFront1(int16_t position);
    inline void setRightFront1(int16_t position);
    inline void setRightRear1(int16_t position);
    inline void setLeftRear1(int16_t position);

    inline void setLeftFront2(int16_t position);
    inline void setRightFront2(int16_t position);
    inline void setRightRear2(int16_t position);
    inline void setLeftRear2(int16_t position);

    void setServoSpeed(uint16_t speed);
    void setServoSpeedJoin0(uint16_t speed);
    void setServoSpeedJoin1(uint16_t speed);
    void setServoSpeedJoin2(uint16_t speed);

};



#endif //RobotControl_H_