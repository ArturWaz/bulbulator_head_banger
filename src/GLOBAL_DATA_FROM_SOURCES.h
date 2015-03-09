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
    static const size_t bufferLength = 5000;

    namespace AccelProcessed {
        static Buffer<Vector3DTime> buffer(bufferLength); // accelerometer measures in [m/s/s]
    }

    namespace GyroProcessed {
        static Buffer<Vector3DTime> buffer(bufferLength);
    }

    namespace MagProcessed {
        static Buffer<Vector3DTime> buffer(bufferLength);
    }

    namespace EulerAngle {
        static Buffer<Vector3DTime> buffer(bufferLength); // euler angles measures in [rad]
    }

    namespace Quaternion {
        static Buffer<Vector3DTime> buffer(bufferLength);
    }

}


#endif //__GLOBAL_DATA_FROM_SOURCES_H_