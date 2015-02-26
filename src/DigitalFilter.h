/*
 * DigitalFilter.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#ifndef DigitalFilter_H_
#define DigitalFilter_H_


#include "Buffer.h"



//#include "iir_filter1.h"
#include "iir_filter2.h"
#include "lin_high_pass_filter.h"


namespace DigitalFilter {


    template <class T, unsigned int NUMBER_OF_SECTIONS> class IIR_DirectFormII {

        unsigned int numberOfSections;
        Buffer<T,3> buffer[NUMBER_OF_SECTIONS];

//        T **numerator;
//        T **denominator;

    public:

        IIR_DirectFormII(): numberOfSections(NUMBER_OF_SECTIONS) {}
        ~IIR_DirectFormII() {}

        T filterMatlabCoeffs(T const &actualValue) {
            T out = actualValue;

            for (int i = 0; i < numberOfSections; ++i) {

                buffer[i].push( filterFromMatlab2::NUM[i*2][0]*out - filterFromMatlab2::DEN[i*2+1][1]*buffer[i](1) - filterFromMatlab2::DEN[i*2+1][2]*buffer[i](2) );
                out = filterFromMatlab2::NUM[i*2+1][0]*buffer[i](0) + filterFromMatlab2::NUM[i*2+1][1]*buffer[i](1) + filterFromMatlab2::NUM[i*2+1][2]*buffer[i](2);

            }

            return out;
        }

    };



    template <class T, unsigned int ORDER> class FIR_DirectFormI {

        Buffer<T,ORDER> buffer;
        unsigned int order;

//        T *numerator;

    public:

        FIR_DirectFormI(): order(ORDER) {}
        ~FIR_DirectFormI() {}

        T filterMatlabCoeffs(T const &actualValue) {
            T out = 0.0;

            buffer.push(actualValue);

            for (unsigned int i = 0; i < order; ++i) {

                out += buffer(i)*FIRfilter1::B[i];

            }

            return out;
        }

    };


    template <class T, unsigned int ORDER> class Average {

        Buffer<T,ORDER> buffer;
        unsigned int order;

        T sum; // temporary average use to improve speed of calculations

    public:

        Average(): order(ORDER), sum(0) {}
        ~Average() {}

        T filter(T const &actualValue) {
            sum -= buffer(order-1);
            buffer.push(actualValue);
            sum += buffer(0);
            return sum/order;
        }

    };


}

#endif