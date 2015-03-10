/*
 * DigitalFilter.h
 *
 *  Created on: 14.02.15
 *      Author: artur
 */


#ifndef DigitalFilter_H_
#define DigitalFilter_H_


#include "Buffer.h"
#include <list>



//#include "iir_filter1.h"
#include "iir_filter2.h"
#include "lin_high_pass_filter.h"


namespace DigitalFilter {


    template <class T, unsigned int NUMBER_OF_SECTIONS> class IIR_DirectFormII {

        unsigned int numberOfSections;
        Buffer<T> *buffer;

//        T **numerator;
//        T **denominator;

    public:

        IIR_DirectFormII():  numberOfSections(NUMBER_OF_SECTIONS) {
            buffer = new Buffer<T>[numberOfSections](2);
        }
        ~IIR_DirectFormII() { delete []buffer; }

        T filterMatlabCoeffs(T const &actualValue) {
            T out = actualValue;

            for (int i = 0; i < numberOfSections; ++i) {

                T actual = filterFromMatlab2::NUM[i*2][0]*out - filterFromMatlab2::DEN[i*2+1][1]*buffer[i](0) - filterFromMatlab2::DEN[i*2+1][2]*buffer[i](1);
                out = filterFromMatlab2::NUM[i*2+1][0]*actual + filterFromMatlab2::NUM[i*2+1][1]*buffer[i](0) + filterFromMatlab2::NUM[i*2+1][2]*buffer[i](1);

                buffer[i].push(actual);
            }

            return out;
        }

    };



    template <class T, unsigned int ORDER> class FIR_DirectFormI {

        Buffer<T> buffer;
        unsigned int order;

//        T *numerator;

    public:

        FIR_DirectFormI(): buffer(ORDER), order(ORDER) {}
        ~FIR_DirectFormI() {}

        T filterMatlabCoeffs(T const &actualValue) {
            T out = 0.0;

            buffer.push(actualValue);

            for (unsigned int i = 0; i < order; ++i)
                out += buffer(i)*FIRfilter1::B[i];

            return out;
        }

    };


    template <class T, unsigned int ORDER> class Average {

        Buffer<T> buffer;

        T sum; // temporary average use to improve speed of calculations

    public:

        Average(): buffer(ORDER), sum(0) {}
        ~Average() {}

        T filter(T const &actualValue) {
            sum -= buffer(buffer.size()-1);
            buffer.push(actualValue);
            sum += buffer(0);
            return sum/buffer.size();
        }

        inline T actual() const { return sum/buffer.size(); }

    };


    template <class T, unsigned int ORDER> class Median {

        Buffer<T> buffer;
        unsigned int medianIndex;

//        struct element {
//            element *next;
//            T *value;
//        };


    public:

        Median(): buffer(ORDER), medianIndex(ORDER/2) {}
        ~Median() {}

        T filter(T const &actualValue) {
            T out = 0.0;

            buffer.push(actualValue);


            std::list<double> tmpList;

            for (unsigned int i = 0; i < buffer.size(); ++i)
                tmpList.push_back(buffer(i));

            tmpList.sort();

            std::list<double>::iterator it = tmpList.begin();
            for (unsigned int i = 0; i < medianIndex; ++i) ++it;
            out = *it;

//            tmpList.clear();

            return out;
        }

    };

}

#endif