/*
 * GLOBAL_DATA_FROM_SOURCES.h
 *
 *  Created on: 3/6/2015
 *      Author: Artur
 */


#ifndef GLOBAL_DATA_FROM_SOURCES_H_
#define GLOBAL_DATA_FROM_SOURCES_H_


#include "Buffer.h"
#include "SensorsStructures.h"


namespace GlobalData {
    static const size_t buffersLengths = 5000;

    struct AccelProcessed {
        static double lastAcqusisionTime;
        static Buffer<Vector3DTime,buffersLengths> buffer;
    };

    struct GyroProcessed {
        static double lastAcqusisionTime;
        static Buffer<Vector3DTime,buffersLengths> buffer;
    };

    struct MagProcessed {
        static double lastAcqusisionTime;
        static Buffer<Vector3DTime,buffersLengths> buffer;
    };

    struct EulerAngle {
        static double lastAcqusisionTime;
        static Buffer<Vector3DTime,buffersLengths> buffer;
    };

    struct Quaternion {
        static double lastAcqusisionTime;
        static Buffer<Vector3DTime,buffersLengths> buffer;
    };

}


#endif //__GLOBAL_DATA_FROM_SOURCES_H_